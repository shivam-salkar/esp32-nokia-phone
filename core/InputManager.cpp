// =============================================================
// InputManager.cpp — Keypad & Serial Monitor → Abstract Events
// =============================================================

#include "InputManager.h"
#include <Keypad.h>
#include "../config/pins.h"

// Singleton instance
InputManager Input;

// ─────────────────────────────────────────────────────────────
// 4×4 Membrane Keypad (Preserve exact electrical configuration)
// ─────────────────────────────────────────────────────────────
const byte ROWS = KEYPAD_ROWS;
const byte COLS = KEYPAD_COLS;

char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {
  KEYPAD_R1, KEYPAD_R2, KEYPAD_R3, KEYPAD_R4
};

byte colPins[COLS] = {
  KEYPAD_C1, KEYPAD_C2, KEYPAD_C3, KEYPAD_C4
};

static Keypad _keypadInstance = Keypad(
  makeKeymap(keys),
  rowPins,
  colPins,
  ROWS,
  COLS
);

// ─────────────────────────────────────────────────────────────
void InputManager::begin() {
    _pending    = INPUT_NONE;
    _mode       = INPUT_MODE_NAV;
    _lastRawKey = '\0';
    _bufLen     = 0;
    _buf[0]     = '\0';

    Serial.println(F("[INPUT] Initializing keypad..."));
    Serial.println(F("[INPUT] Keypad OK"));
    Serial.println(F("[INPUT] Serial debug input ready"));
    Serial.println(F("[INPUT] Nav: 2=UP 8=DN 4=LT 6=RT 5=OK *=BACK #=MENU 0=HOME"));
}

// ─────────────────────────────────────────────────────────────
void InputManager::update() {
    _processKeypad();
    _processSerial();
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
void InputManager::_processKeypad() {
    char key = _keypadInstance.getKey();
    if (key) {
        _lastRawKey = key;
        InputEvent ev = _mapCharToEvent(key);
        if (ev != INPUT_NONE) {
            _pending = ev;
            Serial.print(F("[INPUT] Keypad: '"));
            Serial.print(key);
            Serial.print(F("' -> Event: "));
            Serial.println((int)ev);
        }
    }
}

// ─────────────────────────────────────────────────────────────
InputEvent InputManager::_mapCharToEvent(char key) {
    if (_mode == INPUT_MODE_NAV) {
        switch (key) {
            case '2': return INPUT_UP;
            case '8': return INPUT_DOWN;
            case '4': return INPUT_LEFT;
            case '6': return INPUT_RIGHT;
            case '5': return INPUT_OK;
            case '*': return INPUT_BACK;
            case '#': return INPUT_MENU;
            case '0': return INPUT_HOME;

            case '1': return INPUT_KEY_1;
            case '3': return INPUT_KEY_3;
            case '7': return INPUT_KEY_7;
            case '9': return INPUT_KEY_9;

            case 'A': return INPUT_A;
            case 'B': return INPUT_B;
            case 'C': return INPUT_C;
            case 'D': return INPUT_D;
            default:  return INPUT_NONE;
        }
    } else {
        // NUMERIC / TEXT MODE
        switch (key) {
            case '0': return INPUT_KEY_0;
            case '1': return INPUT_KEY_1;
            case '2': return INPUT_KEY_2;
            case '3': return INPUT_KEY_3;
            case '4': return INPUT_KEY_4;
            case '5': return INPUT_KEY_5;
            case '6': return INPUT_KEY_6;
            case '7': return INPUT_KEY_7;
            case '8': return INPUT_KEY_8;
            case '9': return INPUT_KEY_9;

            case 'A': return INPUT_A;
            case 'B': return INPUT_B;
            case 'C': return INPUT_C;
            case 'D': return INPUT_D;

            case '*': return INPUT_STAR;
            case '#': return INPUT_HASH;
            default:  return INPUT_NONE;
        }
    }
}

// ─────────────────────────────────────────────────────────────
// Non-blocking Serial reader
// ─────────────────────────────────────────────────────────────
void InputManager::_processSerial() {
    while (Serial.available()) {
        char c = (char)Serial.read();

        if (c == '\n' || c == '\r') {
            if (_bufLen > 0) {
                _buf[_bufLen] = '\0';
                String token(_buf);
                token.trim();
                token.toUpperCase();
                _bufLen = 0;

                if (token.length() == 0) continue;

                InputEvent ev = _parseToken(token);
                if (ev != INPUT_NONE) {
                    _pending = ev;
                    Serial.print(F("[INPUT] Serial: "));
                    Serial.println(token);
                } else {
                    Serial.print(F("[INPUT] Unknown Serial command: "));
                    Serial.println(token);
                }
            }
        } else {
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
    if (token == "HOME"  || token == "H")  return INPUT_HOME;

    if (token.length() == 1) {
        char c = token.charAt(0);
        _lastRawKey = c;
        return _mapCharToEvent(c);
    }

    return INPUT_NONE;
}
