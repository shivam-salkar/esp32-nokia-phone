// =============================================================
// TextEditor.h — Note-taking app with LittleFS persistence
// =============================================================

#ifndef TEXT_EDITOR_H
#define TEXT_EDITOR_H

#include "../../core/AppManager.h"
#include "../../core/InputManager.h"
#include <LittleFS.h>

class TextEditor : public App {
public:
    void begin()  override;
    void update() override;
    void onInput(InputEvent ev) override;

private:
    // Sub-modes within the editor
    enum EditorMode {
        MODE_FILE_LIST,   // Browse saved notes
        MODE_VIEWING,     // View a note
        MODE_EDITING,     // Type a new note (via Serial)
        MODE_CONFIRM_DEL  // Confirm deletion
    };

    EditorMode _mode;

    // File list
    static const uint8_t MAX_FILES = 8;
    char    _files[MAX_FILES][24];
    uint8_t _fileCount;
    uint8_t _fileSelected;

    // Current note buffer
    char    _noteBuf[512];
    uint16_t _noteLen;
    char    _currentFile[32];

    // Serial input accumulation for text entry
    bool    _waitingForText;
    char    _serialBuf[512];
    uint16_t _serialLen;

    void _drawFileList();
    void _drawViewer();
    void _drawEditor();
    void _drawConfirmDelete();

    void _scanFiles();
    bool _loadFile(const char* filename);
    bool _saveFile(const char* filename, const char* content);
    bool _deleteFile(const char* filename);
    void _processSerialText();

    void _showMsg(const char* msg, uint16_t colour = C_WHITE);
};

#endif // TEXT_EDITOR_H
