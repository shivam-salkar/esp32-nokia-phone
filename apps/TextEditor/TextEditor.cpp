// =============================================================
// TextEditor.cpp — Notes App with Multi-Tap Text & LittleFS
// =============================================================

#include "TextEditor.h"
#include "../../core/DisplayManager.h"
#include "../../core/AppManager.h"
#include "../../storage/StorageManager.h"

static const char* NOTES_DIR = "/notes";
static const uint32_t MULTITAP_TIMEOUT_MS = 800;

// ─────────────────────────────────────────────────────────────
void TextEditor::begin() {
    Serial.println(F("[APP] Notes / TextEditor started"));
    Input.setMode(INPUT_MODE_NAV);

    _mode           = MODE_FILE_LIST;
    _fileCount      = 0;
    _fileSelected   = 0;
    _noteLen        = 0;
    _noteBuf[0]     = '\0';
    _currentFile[0] = '\0';

    _lastKey        = '\0';
    _tapIndex       = 0;
    _lastTapTime    = 0;
    _hasPendingChar = false;
    _capsLock       = true;
    _serialLen      = 0;

    _scanFiles();
    _drawFileList();
}

// ─────────────────────────────────────────────────────────────
void TextEditor::update() {
    if (_mode == MODE_EDITING) {
        if (_hasPendingChar && (millis() - _lastTapTime >= MULTITAP_TIMEOUT_MS)) {
            _commitPending();
            _drawEditor();
        }
        _processSerialText();
    }
}

// ─────────────────────────────────────────────────────────────
void TextEditor::onInput(InputEvent ev) {
    if (_mode == MODE_FILE_LIST) {
        switch (ev) {
            case INPUT_UP:
            case INPUT_KEY_2:
                if (_fileSelected > 0) {
                    _fileSelected--;
                    _drawFileList();
                }
                break;

            case INPUT_DOWN:
            case INPUT_KEY_8:
                if (_fileSelected + 1 < _fileCount) {
                    _fileSelected++;
                    _drawFileList();
                }
                break;

            case INPUT_OK:
            case INPUT_KEY_5:
                if (_fileCount > 0) {
                    _loadFile(_files[_fileSelected]);
                    _mode = MODE_VIEWING;
                    _drawViewer();
                }
                break;

            case INPUT_MENU:
            case INPUT_HASH:
            case INPUT_A:
                // New Note
                _noteLen        = 0;
                _noteBuf[0]     = '\0';
                snprintf(_currentFile, sizeof(_currentFile), "note_%u.txt", (unsigned)millis() % 10000);
                _mode           = MODE_EDITING;
                Input.setMode(INPUT_MODE_TEXT);
                _lastKey        = '\0';
                _hasPendingChar = false;
                _drawEditor();
                break;

            case INPUT_BACK:
            case INPUT_STAR:
                AppMgr.launchApp(APP_MENU);
                break;

            case INPUT_HOME:
            case INPUT_KEY_0:
                AppMgr.launchApp(APP_LAUNCHER);
                break;

            default:
                break;
        }
    } else if (_mode == MODE_VIEWING) {
        switch (ev) {
            case INPUT_OK:
            case INPUT_KEY_5:
                // Edit note
                _mode = MODE_EDITING;
                Input.setMode(INPUT_MODE_TEXT);
                _lastKey = '\0';
                _hasPendingChar = false;
                _drawEditor();
                break;

            case INPUT_B:
            case INPUT_C:
                // Delete
                _mode = MODE_CONFIRM_DEL;
                _drawConfirmDelete();
                break;

            case INPUT_BACK:
            case INPUT_STAR:
                _mode = MODE_FILE_LIST;
                _scanFiles();
                _drawFileList();
                break;

            case INPUT_HOME:
            case INPUT_KEY_0:
                AppMgr.launchApp(APP_LAUNCHER);
                break;

            default:
                break;
        }
    } else if (_mode == MODE_EDITING) {
        // Multi-tap text editing mode
        char raw = Input.getLastRawKey();

        if (raw >= '0' && raw <= '9') {
            _handleMultiTap(raw);
        } else if (ev == INPUT_C) {
            // Toggle Caps Lock
            _commitPending();
            _capsLock = !_capsLock;
            _drawEditor();
        } else if (ev == INPUT_D) {
            // Newline
            _commitPending();
            if (_noteLen < sizeof(_noteBuf) - 2) {
                _noteBuf[_noteLen++] = '\n';
                _noteBuf[_noteLen]   = '\0';
                _drawEditor();
            }
        } else if (ev == INPUT_HASH || ev == INPUT_A) {
            // Save note
            _commitPending();
            _saveFile(_currentFile, _noteBuf);
            Input.setMode(INPUT_MODE_NAV);
            _mode = MODE_FILE_LIST;
            _scanFiles();
            _drawFileList();
        } else if (ev == INPUT_STAR || ev == INPUT_BACK) {
            // Backspace / Cancel
            if (_hasPendingChar) {
                _hasPendingChar = false;
                if (_noteLen > 0) _noteBuf[--_noteLen] = '\0';
                _lastKey = '\0';
                _drawEditor();
            } else if (_noteLen > 0) {
                _noteBuf[--_noteLen] = '\0';
                _drawEditor();
            } else {
                // Empty note — exit edit mode
                Input.setMode(INPUT_MODE_NAV);
                _mode = MODE_FILE_LIST;
                _drawFileList();
            }
        }
    } else if (_mode == MODE_CONFIRM_DEL) {
        if (ev == INPUT_OK || ev == INPUT_KEY_5 || ev == INPUT_B) {
            _deleteFile(_currentFile);
            _mode = MODE_FILE_LIST;
            _scanFiles();
            _drawFileList();
        } else if (ev == INPUT_BACK || ev == INPUT_STAR) {
            _mode = MODE_VIEWING;
            _drawViewer();
        }
    }
}

// ─────────────────────────────────────────────────────────────
char TextEditor::_getMultiTapChar(char key, uint8_t index) {
    const char* letters = "";
    switch (key) {
        case '1': letters = ".,!?-1"; break;
        case '2': letters = _capsLock ? "ABC2" : "abc2"; break;
        case '3': letters = _capsLock ? "DEF3" : "def3"; break;
        case '4': letters = _capsLock ? "GHI4" : "ghi4"; break;
        case '5': letters = _capsLock ? "JKL5" : "jkl5"; break;
        case '6': letters = _capsLock ? "MNO6" : "mno6"; break;
        case '7': letters = _capsLock ? "PQRS7" : "pqrs7"; break;
        case '8': letters = _capsLock ? "TUV8" : "tuv8"; break;
        case '9': letters = _capsLock ? "WXYZ9" : "wxyz9"; break;
        case '0': letters = " 0"; break;
        default: return '\0';
    }

    uint8_t count = strlen(letters);
    if (count == 0) return '\0';
    return letters[index % count];
}

void TextEditor::_commitPending() {
    _hasPendingChar = false;
    _lastKey        = '\0';
    _tapIndex       = 0;
}

void TextEditor::_handleMultiTap(char key) {
    uint32_t now = millis();

    if (_hasPendingChar && key == _lastKey && (now - _lastTapTime < MULTITAP_TIMEOUT_MS)) {
        // Repeated tap on same key — cycle through letters
        _tapIndex++;
        char c = _getMultiTapChar(key, _tapIndex);
        if (_noteLen > 0) {
            _noteBuf[_noteLen - 1] = c;
        }
        _lastTapTime = now;
        _drawEditor();
    } else {
        // Different key or timeout: commit previous and start new character
        _commitPending();

        if (_noteLen < sizeof(_noteBuf) - 2) {
            _lastKey        = key;
            _tapIndex       = 0;
            _lastTapTime    = now;
            _hasPendingChar = true;

            char c = _getMultiTapChar(key, _tapIndex);
            _noteBuf[_noteLen++] = c;
            _noteBuf[_noteLen]   = '\0';
            _drawEditor();
        }
    }
}

// ─────────────────────────────────────────────────────────────
void TextEditor::_scanFiles() {
    _fileCount = 0;
    if (!Storage.isLittleFSReady()) return;

    File root = LittleFS.open(NOTES_DIR);
    if (!root || !root.isDirectory()) return;

    File file = root.openNextFile();
    while (file && _fileCount < MAX_FILES) {
        if (!file.isDirectory()) {
            const char* name = file.name();
            // Store simple filename
            const char* slash = strrchr(name, '/');
            strncpy(_files[_fileCount], slash ? slash + 1 : name, sizeof(_files[_fileCount]) - 1);
            _files[_fileCount][sizeof(_files[_fileCount]) - 1] = '\0';
            _fileCount++;
        }
        file = root.openNextFile();
    }

    if (_fileSelected >= _fileCount && _fileCount > 0) {
        _fileSelected = _fileCount - 1;
    }
}

bool TextEditor::_loadFile(const char* filename) {
    char path[48];
    snprintf(path, sizeof(path), "%s/%s", NOTES_DIR, filename);
    strncpy(_currentFile, filename, sizeof(_currentFile) - 1);

    File f = LittleFS.open(path, "r");
    if (!f) return false;

    _noteLen = 0;
    while (f.available() && _noteLen < sizeof(_noteBuf) - 1) {
        _noteBuf[_noteLen++] = f.read();
    }
    _noteBuf[_noteLen] = '\0';
    f.close();
    return true;
}

bool TextEditor::_saveFile(const char* filename, const char* content) {
    char path[48];
    snprintf(path, sizeof(path), "%s/%s", NOTES_DIR, filename);

    File f = LittleFS.open(path, "w");
    if (!f) return false;

    f.print(content);
    f.close();
    return true;
}

bool TextEditor::_deleteFile(const char* filename) {
    char path[48];
    snprintf(path, sizeof(path), "%s/%s", NOTES_DIR, filename);
    return LittleFS.remove(path);
}

// ─────────────────────────────────────────────────────────────
void TextEditor::_drawFileList() {
    Display.clear(C_BLACK);
    Display.drawHeader("NOTES", C_NOKIA_BLUE, C_WHITE);

    if (_fileCount == 0) {
        Display.setTextSize(1);
        Display.printCentered("No Notes Found", 50, 1, C_LIGHT_GREY, C_BLACK);
        Display.printCentered("Press # to create new", 70, 1, C_NOKIA_CYAN, C_BLACK);
    } else {
        for (uint8_t i = 0; i < _fileCount; i++) {
            int16_t y   = 18 + i * 15;
            bool    hl  = (i == _fileSelected);
            uint16_t bg = hl ? C_NOKIA_BLUE : C_BLACK;
            uint16_t fg = hl ? C_WHITE : C_LIGHT_GREY;

            Display.fillRect(2, y, SCREEN_W - 4, 14, bg);
            if (hl) Display.drawRect(2, y, SCREEN_W - 4, 14, C_NOKIA_CYAN);

            Display.setTextSize(1);
            Display.setTextColour(fg, bg);
            Display.setCursor(6, y + 3);
            Display.print(_files[i]);
        }
    }

    Display.drawDivider(136, C_DARK_GREY);
    Display.printCentered("#:New  5:Open  *:Menu", 138, 1, C_LIGHT_GREY, C_BLACK);
    Display.drawSoftKeys("#: New", "5: Open", C_NOKIA_BLUE, C_WHITE);
}

void TextEditor::_drawViewer() {
    Display.clear(C_BLACK);
    Display.drawHeader(_currentFile, C_NOKIA_BLUE, C_WHITE);

    // Note contents box
    Display.drawRect(2, 16, SCREEN_W - 4, 118, C_DARK_GREY);
    Display.setTextSize(1);
    Display.setTextColour(C_WHITE, C_BLACK);
    Display.setCursor(5, 20);

    // Simple line printing
    int16_t curX = 5, curY = 20;
    for (uint16_t i = 0; i < _noteLen; i++) {
        char c = _noteBuf[i];
        if (c == '\n' || curX > SCREEN_W - 12) {
            curX = 5;
            curY += 10;
            if (curY > 124) break;
            if (c == '\n') continue;
        }
        Display.setCursor(curX, curY);
        char s[2] = { c, '\0' };
        Display.print(s);
        curX += 6;
    }

    Display.drawDivider(136, C_DARK_GREY);
    Display.printCentered("5:Edit  B:Del  *:Back", 138, 1, C_LIGHT_GREY, C_BLACK);
    Display.drawSoftKeys("*: Back", "5: Edit", C_NOKIA_BLUE, C_WHITE);
}

void TextEditor::_drawEditor() {
    Display.clear(C_BLACK);

    // Header showing filename and Caps status
    char head[24];
    snprintf(head, sizeof(head), "EDIT [%s]", _capsLock ? "ABC" : "abc");
    Display.drawHeader(head, C_NOKIA_BLUE, C_WHITE);

    // Text box
    Display.drawRect(2, 16, SCREEN_W - 4, 102, C_NOKIA_BLUE);
    Display.setTextSize(1);
    Display.setTextColour(C_WHITE, C_BLACK);

    int16_t curX = 5, curY = 20;
    for (uint16_t i = 0; i < _noteLen; i++) {
        char c = _noteBuf[i];
        if (c == '\n' || curX > SCREEN_W - 12) {
            curX = 5;
            curY += 10;
            if (curY > 110) break;
            if (c == '\n') continue;
        }
        Display.setCursor(curX, curY);
        char s[2] = { c, '\0' };
        Display.print(s);
        curX += 6;
    }

    // Blinking / pending cursor indicator
    if (_hasPendingChar) {
        Display.fillRect(curX, curY + 6, 5, 2, C_YELLOW);
    } else {
        Display.fillRect(curX, curY + 6, 5, 2, C_NOKIA_CYAN);
    }

    // Multi-tap helper
    Display.drawDivider(120, C_DARK_GREY);
    Display.setTextSize(1);
    Display.printCentered("2-9:Type  0:Spc  *:Bk", 123, 1, C_LIGHT_GREY, C_BLACK);
    Display.printCentered("#:Save  C:Caps  D:Enter", 135, 1, C_NOKIA_CYAN, C_BLACK);

    Display.drawSoftKeys("*: Backspace", "#: Save", C_NOKIA_BLUE, C_WHITE);
}

void TextEditor::_drawConfirmDelete() {
    Display.clear(C_BLACK);
    Display.drawHeader("DELETE NOTE", C_RED, C_WHITE);

    Display.printCentered("Delete this note?", 50, 1, C_WHITE, C_BLACK);
    Display.printCentered(_currentFile, 68, 1, C_YELLOW, C_BLACK);

    Display.printCentered("Press 5 to Confirm", 96, 1, C_RED, C_BLACK);
    Display.printCentered("Press * to Cancel", 112, 1, C_LIGHT_GREY, C_BLACK);

    Display.drawSoftKeys("*: Cancel", "5: Delete", C_RED, C_WHITE);
}

// ─────────────────────────────────────────────────────────────
void TextEditor::_processSerialText() {
    while (Serial.available()) {
        char c = (char)Serial.read();
        if (c == '\n' || c == '\r') {
            if (_serialLen > 0) {
                _serialBuf[_serialLen] = '\0';
                String line(_serialBuf);
                line.trim();
                _serialLen = 0;

                if (line.equalsIgnoreCase("SAVE")) {
                    _saveFile(_currentFile, _noteBuf);
                    Input.setMode(INPUT_MODE_NAV);
                    _mode = MODE_FILE_LIST;
                    _scanFiles();
                    _drawFileList();
                    return;
                } else if (line.equalsIgnoreCase("CANCEL")) {
                    Input.setMode(INPUT_MODE_NAV);
                    _mode = MODE_FILE_LIST;
                    _drawFileList();
                    return;
                } else {
                    // Append line to note
                    for (unsigned int i = 0; i < line.length(); i++) {
                        if (_noteLen < sizeof(_noteBuf) - 2) {
                            _noteBuf[_noteLen++] = line.charAt(i);
                        }
                    }
                    _noteBuf[_noteLen] = '\0';
                    _drawEditor();
                }
            }
        } else {
            if (_serialLen < sizeof(_serialBuf) - 1) {
                _serialBuf[_serialLen++] = c;
            }
        }
    }
}
