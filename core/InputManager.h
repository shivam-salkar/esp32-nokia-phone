// =============================================================
// InputManager.h — Abstract Input Event System
// =============================================================
// Keypad & Serial Monitor → InputManager → InputEvent → Active App
// =============================================================

#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <Arduino.h>

// ─────────────────────────────────────────────────────────────
// Abstract input events — required by OS and apps
// ─────────────────────────────────────────────────────────────
enum InputEvent {
    INPUT_NONE = 0,

    INPUT_UP,
    INPUT_DOWN,
    INPUT_LEFT,
    INPUT_RIGHT,

    INPUT_OK,
    INPUT_BACK,
    INPUT_MENU,
    INPUT_HOME,

    INPUT_KEY_0,
    INPUT_KEY_1,
    INPUT_KEY_2,
    INPUT_KEY_3,
    INPUT_KEY_4,
    INPUT_KEY_5,
    INPUT_KEY_6,
    INPUT_KEY_7,
    INPUT_KEY_8,
    INPUT_KEY_9,

    INPUT_A,
    INPUT_B,
    INPUT_C,
    INPUT_D,
    INPUT_STAR,
    INPUT_HASH
};

// Input modes: how physical keys are translated
enum InputMode {
    INPUT_MODE_NAV,       // 2=UP, 8=DOWN, 4=LEFT, 6=RIGHT, 5=OK, *=BACK, #=MENU, 0=HOME
    INPUT_MODE_NUMERIC,   // '0'-'9' -> INPUT_KEY_0..9, * -> INPUT_STAR, # -> INPUT_HASH
    INPUT_MODE_TEXT       // For multi-tap text entry
};

class InputManager {
public:
    void begin();
    void update();
    InputEvent getEvent();
    bool hasEvent() const;

    void setMode(InputMode mode) { _mode = mode; }
    InputMode getMode() const { return _mode; }

    char getLastRawKey() const { return _lastRawKey; }

    static bool isDigit(InputEvent ev) {
        return (ev >= INPUT_KEY_0 && ev <= INPUT_KEY_9);
    }

    static char eventToChar(InputEvent ev) {
        if (ev >= INPUT_KEY_0 && ev <= INPUT_KEY_9) return '0' + (ev - INPUT_KEY_0);
        switch (ev) {
            case INPUT_A: return 'A';
            case INPUT_B: return 'B';
            case INPUT_C: return 'C';
            case INPUT_D: return 'D';
            case INPUT_STAR: return '*';
            case INPUT_HASH: return '#';
            default: return '\0';
        }
    }

private:
    InputEvent _pending;
    InputMode  _mode;
    char       _lastRawKey;

    // Non-blocking serial receive buffer
    static const uint8_t BUF_SIZE = 32;
    char    _buf[BUF_SIZE];
    uint8_t _bufLen;

    void _processKeypad();
    void _processSerial();
    InputEvent _mapCharToEvent(char key);
    InputEvent _parseToken(const String& token);
};

// Singleton
extern InputManager Input;

#endif // INPUT_MANAGER_H
