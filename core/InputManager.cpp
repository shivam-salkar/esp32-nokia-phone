// =============================================================
// InputManager.cpp — Serial Monitor → Abstract Input Events
// =============================================================
// Uses a non-blocking character accumulator instead of
// readStringUntil(), which can stall on ESP32-S3 USB-CDC.
//
// Accepted Serial commands (case-insensitive):
//   UP / u       → INPUT_UP
//   DOWN / d     → INPUT_DOWN
//   LEFT / l     → INPUT_LEFT
//   RIGHT / r    → INPUT_RIGHT
//   OK / o       → INPUT_OK
//   BACK / b     → INPUT_BACK
//   MENU / m     → INPUT_MENU
// =============================================================

#include "InputManager.h"

// Singleton instance
InputManager Input;

// ─────────────────────────────────────────────────────────────
void InputManager::begin() {
    _pending = INPUT_NONE;
    _bufLen  = 0;
    _buf[0]  = '\0';

    Serial.println(F("[INPUT] Serial input ready"));
    Serial.println(F("[INPUT] Commands: UP DOWN LEFT RIGHT OK BACK MENU"));
    Serial.println(F("[INPUT] Aliases : u  d    l    r     o  b    m   "));
}

// ─────────────────────────────────────────────────────────────
void InputManager::update() {
    _processSerial();
    // Future: _processButtons();
}

// ─────────────────────────────────────────────────────────────
InputEvent InputManager::getEvent() {
    InputEvent ev = _pending;
    _pending = INPUT_NONE;
    return ev;
}

// ─────────────────────────────────────────────────────────────
bool InputManager::hasEvent() const {
    return _pending != INPUT_NONE;
}

// ─────────────────────────────────────────────────────────────
// Non-blocking Serial reader
// Accumulates characters into _buf.
// Fires when '\n' or '\r' is received (handles both \n and \r\n).
// ─────────────────────────────────────────────────────────────
void InputManager::_processSerial() {
    while (Serial.available()) {
        char c = (char)Serial.read();

        if (c == '\n' || c == '\r') {
            // End of line — process whatever is in the buffer
            if (_bufLen > 0) {
                _buf[_bufLen] = '\0';
                String token(_buf);
                token.trim();
                token.toUpperCase();
                _bufLen = 0;  // Reset buffer immediately

                if (token.length() == 0) continue;

                InputEvent ev = _parseToken(token);
                if (ev != INPUT_NONE) {
                    _pending = ev;
                    Serial.print(F("[INPUT] Received: "));
                    Serial.println(token);
                } else {
                    Serial.print(F("[INPUT] Unknown command: "));
                    Serial.println(token);
                }
            }
        } else {
            // Accumulate character (drop if buffer full)
            if (_bufLen < BUF_SIZE - 1) {
                _buf[_bufLen++] = c;
            }
        }
    }
}

// ─────────────────────────────────────────────────────────────
InputEvent InputManager::_parseToken(const String& token) {
    if (token == "UP"    || token == "U")  return INPUT_UP;
    if (token == "DOWN"  || token == "D")  return INPUT_DOWN;
    if (token == "LEFT"  || token == "L")  return INPUT_LEFT;
    if (token == "RIGHT" || token == "R")  return INPUT_RIGHT;
    if (token == "OK"    || token == "O")  return INPUT_OK;
    if (token == "BACK"  || token == "B")  return INPUT_BACK;
    if (token == "MENU"  || token == "M")  return INPUT_MENU;
    return INPUT_NONE;
}
