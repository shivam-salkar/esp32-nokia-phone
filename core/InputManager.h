// =============================================================
// InputManager.h — Abstract Input Event System
// =============================================================
// All applications consume InputEvent values.
// No application should ever read Serial or digitalRead directly.
// =============================================================

#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <Arduino.h>

// ─────────────────────────────────────────────────────────────
// Abstract input events — the only thing apps ever see
// ─────────────────────────────────────────────────────────────
enum InputEvent {
    INPUT_NONE,
    INPUT_UP,
    INPUT_DOWN,
    INPUT_LEFT,
    INPUT_RIGHT,
    INPUT_OK,
    INPUT_BACK,
    INPUT_MENU
};

// ─────────────────────────────────────────────────────────────
// InputManager
// ─────────────────────────────────────────────────────────────
class InputManager {
public:
    void begin();           // Call once in setup()
    void update();          // Call every loop() iteration
    InputEvent getEvent();  // Returns pending event (clears it)
    bool hasEvent() const;  // True if an event is waiting

private:
    InputEvent _pending;

    // Non-blocking serial receive buffer
    static const uint8_t BUF_SIZE = 32;
    char    _buf[BUF_SIZE];
    uint8_t _bufLen;

    // Serial input processing
    void _processSerial();
    InputEvent _parseToken(const String& token);
};

// Singleton
extern InputManager Input;

#endif // INPUT_MANAGER_H
