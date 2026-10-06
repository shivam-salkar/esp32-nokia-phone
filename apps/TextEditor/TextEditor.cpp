// =============================================================
// TextEditor.cpp — Note-taking App with LittleFS
// =============================================================
// Notes are stored in LittleFS as /notes/NNN.txt
//
// File list mode:
//   UP/DOWN   → scroll file list
//   OK        → view selected file
//   BACK      → return to menu
//   MENU      → new note (enters edit mode)
//
// View mode:
//   BACK      → return to file list
//   LEFT      → delete note (with confirm)
//
// Edit mode:
//   Type in Serial monitor, press Enter each line.
//   Type "SAVE" (alone on a line) to save.
//   Type "CANCEL" to discard.
//
// Delete confirm:
//   OK        → confirm delete
//   BACK      → cancel
// =============================================================

#include "TextEditor.h"
#include "../../core/DisplayManager.h"
#include "../../core/AppManager.h"

static const char* NOTES_DIR = "/notes";

// ─────────────────────────────────────────────────────────────
void TextEditor::begin() {
    Serial.println(F("[APP] TextEditor started"));

    // Mount LittleFS
    if (!LittleFS.begin(true)) {  // true = format on fail
        Serial.println(F("[FS] LittleFS mount failed"));
        Display.clear(C_BLACK);
        Display.drawHeader("TextEditor", C_NOKIA_BLUE, C_WHITE);
        _showMsg("LittleFS Error!", C_RED);
        return;
    }
    Serial.println(F("[FS] LittleFS mounted"));

    // Create notes directory if absent
    if (!LittleFS.exists(NOTES_DIR)) {
        LittleFS.mkdir(NOTES_DIR);
    }

    _mode         = MODE_FILE_LIST;
    _fileCount    = 0;
    _fileSelected = 0;
    _noteLen      = 0;
    _noteBuf[0]   = '\0';
    _waitingForText = false;
    _serialLen    = 0;
    _serialBuf[0] = '\0';

    _scanFiles();
    _drawFileList();
}

// ─────────────────────────────────────────────────────────────
void TextEditor::update() {
    if (_mode == MODE_EDITING && _waitingForText) {
        _processSerialText();
    }
}

// ─────────────────────────────────────────────────────────────
void TextEditor::onInput(InputEvent ev) {
    switch (_mode) {

        case MODE_FILE_LIST:
            if (ev == INPUT_UP && _fileSelected > 0) {
                _fileSelected--;
                _drawFileList();
            } else if (ev == INPUT_DOWN && _fileSelected < _fileCount - 1) {
                _fileSelected++;
                _drawFileList();
            } else if (ev == INPUT_OK && _fileCount > 0) {
                _loadFile(_files[_fileSelected]);
                _mode = MODE_VIEWING;
                _drawViewer();
            } else if (ev == INPUT_BACK) {
                AppMgr.launchApp(APP_MENU);
            } else if (ev == INPUT_MENU) {
                // New note
                _noteLen      = 0;
                _noteBuf[0]   = '\0';
                _serialLen    = 0;
                _serialBuf[0] = '\0';
                _waitingForText = true;
                _mode = MODE_EDITING;
                _drawEditor();
            }
            break;

        case MODE_VIEWING:
            if (ev == INPUT_BACK) {
                _mode = MODE_FILE_LIST;
                _drawFileList();
            } else if (ev == INPUT_LEFT) {
                _mode = MODE_CONFIRM_DEL;
                _drawConfirmDelete();
            }
            break;

        case MODE_EDITING:
            // Text entry handled via Serial in update()
            // BACK cancels
            if (ev == INPUT_BACK) {
                _waitingForText = false;
                _mode = MODE_FILE_LIST;
                _drawFileList();
            }
            break;

        case MODE_CONFIRM_DEL:
            if (ev == INPUT_OK) {
                _deleteFile(_files[_fileSelected]);
                _scanFiles();
                _mode = MODE_FILE_LIST;
                _drawFileList();
            } else if (ev == INPUT_BACK) {
                _mode = MODE_VIEWING;
                _drawViewer();
            }
            break;
    }
}

// ─────────────────────────────────────────────────────────────
void TextEditor::_scanFiles() {
    _fileCount = 0;
    File dir = LittleFS.open(NOTES_DIR);
    if (!dir || !dir.isDirectory()) return;

    File f = dir.openNextFile();
    while (f && _fileCount < MAX_FILES) {
        if (!f.isDirectory()) {
            // Store just the filename (without dir prefix)
            strncpy(_files[_fileCount], f.name(), 23);
            _files[_fileCount][23] = '\0';
            _fileCount++;
        }
        f = dir.openNextFile();
    }
    Serial.print(F("[FS] Notes found: "));
    Serial.println(_fileCount);
}

// ─────────────────────────────────────────────────────────────
bool TextEditor::_loadFile(const char* filename) {
    char path[40];
    snprintf(path, sizeof(path), "%s/%s", NOTES_DIR, filename);

    File f = LittleFS.open(path, "r");
    if (!f) {
        Serial.print(F("[FS] Cannot open: "));
        Serial.println(path);
        return false;
    }
    _noteLen = 0;
    while (f.available() && _noteLen < 511) {
        _noteBuf[_noteLen++] = (char)f.read();
    }
    _noteBuf[_noteLen] = '\0';
    f.close();
    strncpy(_currentFile, filename, sizeof(_currentFile) - 1);
    return true;
}

// ─────────────────────────────────────────────────────────────
bool TextEditor::_saveFile(const char* filename, const char* content) {
    char path[40];
    snprintf(path, sizeof(path), "%s/%s", NOTES_DIR, filename);

    File f = LittleFS.open(path, "w");
    if (!f) {
        Serial.print(F("[FS] Cannot write: "));
        Serial.println(path);
        return false;
    }
    f.print(content);
    f.close();
    Serial.print(F("[FS] Saved: "));
    Serial.println(path);
    return true;
}

// ─────────────────────────────────────────────────────────────
bool TextEditor::_deleteFile(const char* filename) {
    char path[40];
    snprintf(path, sizeof(path), "%s/%s", NOTES_DIR, filename);
    bool ok = LittleFS.remove(path);
    Serial.print(ok ? F("[FS] Deleted: ") : F("[FS] Delete failed: "));
    Serial.println(path);
    return ok;
}

// ─────────────────────────────────────────────────────────────
// Read Serial lines during editing mode
// Ends when user types "SAVE" or "CANCEL"
// ─────────────────────────────────────────────────────────────
void TextEditor::_processSerialText() {
    if (!Serial.available()) return;

    String line = Serial.readStringUntil('\n');
    line.trim();

    String upper = line;
    upper.toUpperCase();

    if (upper == "SAVE") {
        // Generate filename: noteNNN.txt
        char fname[20];
        snprintf(fname, sizeof(fname), "note%03d.txt", (int)_fileCount + 1);

        _saveFile(fname, _serialBuf);
        _scanFiles();
        _waitingForText = false;
        _mode = MODE_FILE_LIST;
        _drawFileList();
        _showMsg("Note saved!", C_NOKIA_GREEN);
        return;
    }

    if (upper == "CANCEL") {
        _waitingForText = false;
        _mode = MODE_FILE_LIST;
        _drawFileList();
        return;
    }

    // Append line + newline to buffer
    if (_serialLen + line.length() + 1 < 511) {
        strcat(_serialBuf, line.c_str());
        strcat(_serialBuf, "\n");
        _serialLen = strlen(_serialBuf);
    }

    // Update line count display
    Display.setTextSize(1);
    Display.setTextColour(C_NOKIA_GREEN, C_BLACK);
    char info[30];
    snprintf(info, sizeof(info), "%d chars", (int)_serialLen);
    Display.fillRect(0, 140, SCREEN_W, 10, C_BLACK);
    Display.setCursor(2, 140);
    Display.print(info);
}

// ─────────────────────────────────────────────────────────────
void TextEditor::_drawFileList() {
    Display.clear(C_BLACK);
    Display.drawHeader("Text Editor", C_NOKIA_BLUE, C_WHITE);
    Display.drawDivider(16, C_DARK_GREY);

    if (_fileCount == 0) {
        Display.printCentered("No notes yet", 60, 1, C_LIGHT_GREY, C_BLACK);
        Display.printCentered("MENU=New Note", 80, 1, C_DARK_GREY, C_BLACK);
    } else {
        for (uint8_t i = 0; i < _fileCount && i < 6; i++) {
            int16_t y  = 20 + i * 20;
            bool hl    = (i == _fileSelected);
            uint16_t bg = hl ? C_NOKIA_BLUE : C_BLACK;

            Display.fillRect(0, y, SCREEN_W, 18, bg);
            Display.setTextSize(1);
            Display.setTextColour(C_WHITE, bg);
            Display.setCursor(4, y + 5);
            Display.print(hl ? "> " : "  ");
            Display.print(_files[i]);
        }
    }

    // Bottom bar
    Display.fillRect(0, 148, SCREEN_W, 12, C_DARK_GREY);
    Display.setTextColour(C_WHITE, C_DARK_GREY);
    Display.setTextSize(1);
    Display.setCursor(2, 150);
    Display.print("M=New");
    Display.setCursor(SCREEN_W - 34, 150);
    Display.print("B=Back");
}

// ─────────────────────────────────────────────────────────────
void TextEditor::_drawViewer() {
    Display.clear(C_BLACK);
    Display.drawHeader(_currentFile, C_NOKIA_BLUE, C_WHITE);
    Display.drawDivider(16, C_DARK_GREY);

    // Show note content — simple word-wrap at SCREEN_W chars
    Display.setTextSize(1);
    Display.setTextColour(C_WHITE, C_BLACK);

    int16_t x = 2, y = 20;
    uint16_t i = 0;
    while (_noteBuf[i] && y < 140) {
        if (_noteBuf[i] == '\n') {
            x = 2; y += 9; i++;
            continue;
        }
        Display.setCursor(x, y);
        char ch[2] = { _noteBuf[i], '\0' };
        Display.print(ch);
        x += 6;
        if (x >= SCREEN_W - 6) { x = 2; y += 9; }
        i++;
    }

    Display.fillRect(0, 148, SCREEN_W, 12, C_DARK_GREY);
    Display.setTextColour(C_WHITE, C_DARK_GREY);
    Display.setTextSize(1);
    Display.setCursor(2, 150);
    Display.print("L=Del");
    Display.setCursor(SCREEN_W - 34, 150);
    Display.print("B=Back");
}

// ─────────────────────────────────────────────────────────────
void TextEditor::_drawEditor() {
    Display.clear(C_BLACK);
    Display.drawHeader("New Note", C_NOKIA_BLUE, C_WHITE);
    Display.drawDivider(16, C_DARK_GREY);

    Display.printCentered("Type in Serial", 30, 1, C_WHITE, C_BLACK);
    Display.printCentered("then press Enter", 42, 1, C_LIGHT_GREY, C_BLACK);
    Display.printCentered("Type SAVE to save", 60, 1, C_NOKIA_GREEN, C_BLACK);
    Display.printCentered("CANCEL to discard", 72, 1, C_DARK_GREY, C_BLACK);

    Serial.println(F("[EDITOR] Type your note. Enter each line."));
    Serial.println(F("[EDITOR] Type SAVE to save, CANCEL to discard."));
}

// ─────────────────────────────────────────────────────────────
void TextEditor::_drawConfirmDelete() {
    Display.clear(C_BLACK);
    Display.drawHeader("Delete?", C_RED, C_WHITE);
    Display.drawDivider(16, C_DARK_GREY);

    Display.printCentered(_files[_fileSelected], 40, 1, C_WHITE, C_BLACK);
    Display.printCentered("OK=Delete", 70, 1, C_RED, C_BLACK);
    Display.printCentered("BACK=Cancel", 85, 1, C_LIGHT_GREY, C_BLACK);
}

// ─────────────────────────────────────────────────────────────
void TextEditor::_showMsg(const char* msg, uint16_t colour) {
    Display.fillRect(0, 110, SCREEN_W, 20, C_BLACK);
    Display.printCentered(msg, 115, 1, colour, C_BLACK);
    delay(1200);
}
