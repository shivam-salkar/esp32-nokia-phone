// =============================================================
// TextEditor.h — Notes App with Multi-Tap Text & LittleFS
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
    enum EditorMode {
        MODE_FILE_LIST,
        MODE_VIEWING,
        MODE_EDITING,
        MODE_CONFIRM_DEL
    };

    EditorMode _mode;

    // File list management
    static const uint8_t MAX_FILES = 8;
    char    _files[MAX_FILES][24];
    uint8_t _fileCount;
    uint8_t _fileSelected;

    // Current note buffer
    char     _currentFile[32];
    char     _noteBuf[256];
    uint16_t _noteLen;

    // Multi-tap text input engine
    char     _lastKey;
    uint8_t  _tapIndex;
    uint32_t _lastTapTime;
    bool     _hasPendingChar;
    bool     _capsLock;

    // Serial monitor debug input accumulator
    char     _serialBuf[128];
    uint8_t  _serialLen;

    void _scanFiles();
    bool _loadFile(const char* filename);
    bool _saveFile(const char* filename, const char* content);
    bool _deleteFile(const char* filename);

    void _drawFileList();
    void _drawViewer();
    void _drawEditor();
    void _drawConfirmDelete();

    void _handleMultiTap(char key);
    void _commitPending();
    char _getMultiTapChar(char key, uint8_t index);
    void _processSerialText();
};

#endif // TEXT_EDITOR_H
