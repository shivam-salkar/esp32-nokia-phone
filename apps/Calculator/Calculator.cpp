// =============================================================
// Calculator.cpp — 4-function Calculator
// =============================================================
// Keypad layout (4×4):
//   [7][8][9][/]
//   [4][5][6][*]
//   [1][2][3][-]
//   [C][0][=][+]
//
// Serial input commands:
//   UP/DOWN/LEFT/RIGHT  → move cursor on keypad
//   OK                  → press highlighted key
//   BACK                → return to menu
//
// Also accepts direct Serial calculator expressions via typed
// digits at the Serial monitor (for quick testing):
//   The keypad UI is the primary interface.
// =============================================================

#include "Calculator.h"
#include "../../core/DisplayManager.h"
#include "../../core/AppManager.h"

// ─────────────────────────────────────────────────────────────
// Keypad layout — 4 rows × 4 cols, index 0–15
// ─────────────────────────────────────────────────────────────
const char* Calculator::_keyLabels[16] = {
    "7", "8", "9", "/",
    "4", "5", "6", "*",
    "1", "2", "3", "-",
    "C", "0", "=", "+"
};

// Keypad geometry
static const int16_t KP_X  = 2;
static const int16_t KP_Y  = 86;
static const int16_t KP_W  = 30;
static const int16_t KP_H  = 17;
static const int16_t KP_GAP = 1;

// ─────────────────────────────────────────────────────────────
void Calculator::begin() {
    Serial.println(F("[APP] Calculator started"));
    Serial.println(F("[CALC] UP/DOWN/LEFT/RIGHT=move OK=press BACK=menu"));

    _operandA   = 0;
    _operandB   = 0;
    _operator   = 0;
    _enteringB  = false;
    _showResult = false;
    _errorState = false;
    _cursor     = 13; // Start on "0"
    _clearInput();
    _draw();
}

// ─────────────────────────────────────────────────────────────
void Calculator::update() {
    // Nothing to tick
}

// ─────────────────────────────────────────────────────────────
void Calculator::onInput(InputEvent ev) {
    switch (ev) {
        case INPUT_UP:
            if (_cursor >= 4) {
                _highlightKey(_cursor, false);
                _cursor -= 4;
                _highlightKey(_cursor, true);
            }
            break;

        case INPUT_DOWN:
            if (_cursor < 12) {
                _highlightKey(_cursor, false);
                _cursor += 4;
                _highlightKey(_cursor, true);
            }
            break;

        case INPUT_LEFT:
            if (_cursor % 4 != 0) {
                _highlightKey(_cursor, false);
                _cursor--;
                _highlightKey(_cursor, true);
            }
            break;

        case INPUT_RIGHT:
            if (_cursor % 4 != 3) {
                _highlightKey(_cursor, false);
                _cursor++;
                _highlightKey(_cursor, true);
            }
            break;

        case INPUT_OK:
            _pressKey(_cursor);
            break;

        case INPUT_BACK:
            AppMgr.launchApp(APP_MENU);
            return;

        default:
            break;
    }
}

// ─────────────────────────────────────────────────────────────
void Calculator::_pressKey(uint8_t idx) {
    const char* lbl = _keyLabels[idx];

    if (strcmp(lbl, "C") == 0) {
        // Clear all
        _operandA   = 0;
        _operandB   = 0;
        _operator   = 0;
        _enteringB  = false;
        _showResult = false;
        _errorState = false;
        _clearInput();
        _drawDisplay();
        return;
    }

    if (strcmp(lbl, "=") == 0) {
        if (_operator && _enteringB && _inputLen > 0) {
            _operandB   = atof(_input);
            _calculate();
        }
        return;
    }

    // Operator keys
    if (strcmp(lbl, "+") == 0 || strcmp(lbl, "-") == 0 ||
        strcmp(lbl, "*") == 0 || strcmp(lbl, "/") == 0) {
        if (_inputLen > 0 || _showResult) {
            if (!_showResult) _operandA = atof(_input);
            _operator   = lbl[0];
            _enteringB  = true;
            _showResult = false;
            _clearInput();
            _drawDisplay();
        }
        return;
    }

    // Digit/decimal — max 14 chars
    if (_inputLen < 14) {
        // Prevent multiple decimals
        if (strcmp(lbl, ".") == 0) {
            for (uint8_t i = 0; i < _inputLen; i++) {
                if (_input[i] == '.') return;
            }
        }
        _input[_inputLen++] = lbl[0];
        _input[_inputLen]   = '\0';
        _showResult         = false;
        _drawDisplay();
    }
}

// ─────────────────────────────────────────────────────────────
void Calculator::_calculate() {
    double result = 0;
    _errorState   = false;

    switch (_operator) {
        case '+': result = _operandA + _operandB; break;
        case '-': result = _operandA - _operandB; break;
        case '*': result = _operandA * _operandB; break;
        case '/':
            if (_operandB == 0) {
                _errorState = true;
            } else {
                result = _operandA / _operandB;
            }
            break;
    }

    if (_errorState) {
        _clearInput();
        strncpy(_input, "ERR:DIV0", sizeof(_input) - 1);
        _inputLen = 8;
    } else {
        _operandA = result;
        // Format result
        char buf[16];
        if (result == (long)result) {
            snprintf(buf, sizeof(buf), "%ld", (long)result);
        } else {
            snprintf(buf, sizeof(buf), "%.4f", result);
            // Strip trailing zeros
            int8_t i = strlen(buf) - 1;
            while (i > 0 && buf[i] == '0') buf[i--] = '\0';
            if (i > 0 && buf[i] == '.') buf[i] = '\0';
        }
        _clearInput();
        uint8_t len = strlen(buf);
        if (len >= sizeof(_input)) len = sizeof(_input) - 1;
        memcpy(_input, buf, len);
        _inputLen       = len;
        _input[_inputLen] = '\0';
    }

    _showResult = true;
    _operator   = 0;
    _enteringB  = false;

    Serial.print(F("[CALC] Result: "));
    Serial.println(_input);

    _drawDisplay();
}

// ─────────────────────────────────────────────────────────────
void Calculator::_clearInput() {
    _inputLen  = 0;
    _input[0]  = '\0';
}

// ─────────────────────────────────────────────────────────────
void Calculator::_draw() {
    Display.clear(C_BLACK);
    Display.drawHeader("Calculator", C_NOKIA_BLUE, C_WHITE);

    // Display area background
    Display.fillRect(0, 17, SCREEN_W, 66, C_DARK_GREY);
    Display.drawRect(2, 20, SCREEN_W - 4, 60, C_LIGHT_GREY);

    _drawDisplay();
    _drawKeypad();
}

// ─────────────────────────────────────────────────────────────
void Calculator::_drawDisplay() {
    // Clear display content area
    Display.fillRect(3, 21, SCREEN_W - 6, 58, C_BLACK);

    // Show operator / mode hint in small text
    Display.setTextSize(1);
    Display.setTextColour(C_LIGHT_GREY, C_BLACK);
    Display.setCursor(4, 24);
    if (_operator) {
        char hint[4] = { _operandA > 0 ? ' ' : ' ', _operator, ' ', '\0' };
        Display.print(hint);
    }

    // Show A operand (small) if entering B
    if (_enteringB) {
        char aStr[16];
        if (_operandA == (long)_operandA)
            snprintf(aStr, sizeof(aStr), "%.0f %c", _operandA, _operator);
        else
            snprintf(aStr, sizeof(aStr), "%.2f %c", _operandA, _operator);
        Display.setCursor(4, 24);
        Display.print(aStr);
    }

    // Main value in large text
    const char* displayStr = (_inputLen > 0) ? _input : "0";
    uint8_t sz = 2;
    uint8_t len = strlen(displayStr);
    if (len > 7) sz = 1;

    Display.setTextSize(sz);
    uint16_t col = _showResult ? C_NOKIA_GREEN : C_WHITE;
    if (_errorState)  col = C_RED;
    Display.setTextColour(col, C_BLACK);

    int16_t x = SCREEN_W - 4 - (int16_t)len * 6 * sz;
    if (x < 4) x = 4;
    Display.setCursor(x, 48);
    Display.print(displayStr);
}

// ─────────────────────────────────────────────────────────────
void Calculator::_drawKeypad() {
    for (uint8_t i = 0; i < 16; i++) {
        _highlightKey(i, i == _cursor);
    }
}

// ─────────────────────────────────────────────────────────────
void Calculator::_highlightKey(uint8_t idx, bool on) {
    uint8_t col = idx % 4;
    uint8_t row = idx / 4;

    int16_t x = KP_X + col * (KP_W + KP_GAP);
    int16_t y = KP_Y + row * (KP_H + KP_GAP);

    uint16_t bg, fg;

    // Operator keys: column 3
    if (col == 3) {
        bg = on ? C_ORANGE : 0x8000;  // Orange HL vs dark red
        fg = C_WHITE;
    } else if (idx == 12) {
        // C key
        bg = on ? C_RED : 0x4000;
        fg = C_WHITE;
    } else if (idx == 14) {
        // = key
        bg = on ? C_NOKIA_GREEN : 0x0300;
        fg = C_WHITE;
    } else {
        bg = on ? C_NOKIA_BLUE : C_DARK_GREY;
        fg = C_WHITE;
    }

    Display.fillRect(x, y, KP_W, KP_H, bg);
    Display.drawRect(x, y, KP_W, KP_H, C_LIGHT_GREY);

    // Centre label
    uint8_t lblLen = strlen(_keyLabels[idx]);
    int16_t tx = x + (KP_W - lblLen * 6) / 2;
    int16_t ty = y + (KP_H - 8) / 2;
    Display.setTextSize(1);
    Display.setTextColour(fg, bg);
    Display.setCursor(tx, ty);
    Display.print(_keyLabels[idx]);
}
