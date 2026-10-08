// =============================================================
// Calculator.cpp — 4-function Floating-Point Calculator App
// =============================================================

#include "Calculator.h"
#include "../../core/DisplayManager.h"
#include "../../core/AppManager.h"

void Calculator::begin() {
    Serial.println(F("[APP] Calculator started (Numeric Mode)"));
    Input.setMode(INPUT_MODE_NUMERIC);

    _operandA      = 0.0;
    _operandB      = 0.0;
    _operator      = 0;
    _enteringB     = false;
    _showResult    = false;
    _errorState    = false;
    _lastZeroPress = 0;

    _clearAll();
    _draw();
}

void Calculator::update() {
    // Event-driven
}

void Calculator::onInput(InputEvent ev) {
    if (ev >= INPUT_KEY_1 && ev <= INPUT_KEY_9) {
        char d = '1' + (ev - INPUT_KEY_1);
        _lastZeroPress = 0;
        _appendDigit(d);
        return;
    }

    if (ev == INPUT_KEY_0) {
        uint32_t now = millis();
        if (now - _lastZeroPress < 500 && _inputLen > 0 && _input[_inputLen - 1] == '0') {
            // Double-tap zero: turn previous 0 into decimal point
            _input[_inputLen - 1] = '.';
            _drawDisplay();
            _lastZeroPress = 0;
        } else {
            _appendDigit('0');
            _lastZeroPress = now;
        }
        return;
    }

    switch (ev) {
        case INPUT_A:
            _setOperator('+');
            break;

        case INPUT_B:
            _setOperator('-');
            break;

        case INPUT_C:
            _setOperator('*');
            break;

        case INPUT_D:
            _setOperator('/');
            break;

        case INPUT_HASH:
        case INPUT_OK:
            _calculate();
            break;

        case INPUT_STAR:
        case INPUT_BACK:
            if (_inputLen > 0) {
                _backspace();
            } else if (_operator != 0) {
                _operator = 0;
                _enteringB = false;
                _drawDisplay();
            } else {
                AppMgr.launchApp(APP_MENU);
            }
            break;

        case INPUT_HOME:
            AppMgr.launchApp(APP_LAUNCHER);
            break;

        default:
            break;
    }
}

void Calculator::_appendDigit(char digit) {
    if (_showResult || _errorState) {
        _clearAll();
    }

    if (_inputLen < 12) {
        // Replace leading lone '0' unless entering decimal
        if (_inputLen == 1 && _input[0] == '0' && digit != '.') {
            _input[0] = digit;
        } else {
            _input[_inputLen++] = digit;
            _input[_inputLen]   = '\0';
        }
        _drawDisplay();
    }
}

void Calculator::_appendDecimal() {
    if (_showResult || _errorState) {
        _clearAll();
    }

    if (strchr(_input, '.') == nullptr && _inputLen < 11) {
        if (_inputLen == 0) {
            _input[_inputLen++] = '0';
        }
        _input[_inputLen++] = '.';
        _input[_inputLen]   = '\0';
        _drawDisplay();
    }
}

void Calculator::_setOperator(char op) {
    if (_errorState) _clearAll();

    if (_inputLen > 0) {
        if (_enteringB) {
            _calculate();
        }
        _operandA  = atof(_input);
        _enteringB = true;
        _inputLen  = 0;
        _input[0]  = '\0';
    } else if (!_enteringB && _showResult) {
        // Continue operating on result
        _enteringB = true;
    }

    _operator   = op;
    _showResult = false;
    _drawDisplay();
}

void Calculator::_calculate() {
    if (_operator == 0 || _inputLen == 0) return;

    _operandB = atof(_input);
    double res = 0.0;

    switch (_operator) {
        case '+': res = _operandA + _operandB; break;
        case '-': res = _operandA - _operandB; break;
        case '*': res = _operandA * _operandB; break;
        case '/':
            if (_operandB == 0.0) {
                _errorState = true;
                _drawDisplay();
                return;
            }
            res = _operandA / _operandB;
            break;
        default: return;
    }

    _operandA   = res;
    _operator   = 0;
    _enteringB  = false;
    _showResult = true;

    // Format result into _input
    dtostrf(res, 1, 4, _input);
    // Trim trailing zeroes after decimal point
    char* dot = strchr(_input, '.');
    if (dot) {
        char* end = _input + strlen(_input) - 1;
        while (end > dot && *end == '0') {
            *end = '\0';
            end--;
        }
        if (end == dot) *dot = '\0';
    }
    _inputLen = strlen(_input);

    _drawDisplay();
}

void Calculator::_clearAll() {
    _operandA   = 0.0;
    _operandB   = 0.0;
    _operator   = 0;
    _enteringB  = false;
    _showResult = false;
    _errorState = false;
    _inputLen   = 0;
    _input[0]   = '\0';
    _drawDisplay();
}

void Calculator::_backspace() {
    if (_inputLen > 0) {
        _input[--_inputLen] = '\0';
        _drawDisplay();
    }
}

void Calculator::_draw() {
    Display.clear(C_BLACK);
    Display.drawHeader("CALCULATOR", C_NOKIA_BLUE, C_WHITE);
    _drawDisplay();
    _drawKeypadHelp();
}

void Calculator::_drawDisplay() {
    // Screen area y=16..54
    Display.fillRect(2, 16, SCREEN_W - 4, 38, C_BLACK);
    Display.drawRect(2, 16, SCREEN_W - 4, 38, C_NOKIA_BLUE);

    // Operator & Operand A row
    Display.setTextSize(1);
    Display.setTextColour(C_LIGHT_GREY, C_BLACK);
    Display.setCursor(6, 20);

    if (_enteringB || _operator != 0) {
        char topBuf[24];
        char opSym = (_operator == '*') ? 'x' : (_operator == '/') ? '/' : _operator;
        char numBuf[16];
        dtostrf(_operandA, 1, 2, numBuf);
        snprintf(topBuf, sizeof(topBuf), "%s %c", numBuf, opSym);
        Display.print(topBuf);
    }

    // Main value row
    Display.setCursor(6, 34);
    if (_errorState) {
        Display.setTextColour(C_RED, C_BLACK);
        Display.setTextSize(2);
        Display.print("Error /0");
    } else {
        Display.setTextColour(C_WHITE, C_BLACK);
        Display.setTextSize(2);
        if (_inputLen == 0) {
            Display.print(_showResult ? _input : "0");
        } else {
            Display.print(_input);
        }
    }
}

void Calculator::_drawKeypadHelp() {
    // Matrix legend y=56..132
    int16_t startY = 56;
    int16_t btnW   = 29;
    int16_t btnH   = 17;
    int16_t padX   = 4;

    const char* keys[4][4] = {
        {"1", "2", "3", "A:+"},
        {"4", "5", "6", "B:-"},
        {"7", "8", "9", "C:x"},
        {"*", "0", "#:=", "D:/"}
    };

    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            int16_t x = padX + c * (btnW + 2);
            int16_t y = startY + r * (btnH + 2);

            uint16_t bg = (c == 3 || (r == 3 && c != 1)) ? C_NOKIA_BLUE : C_DARK_GREY;
            Display.fillRect(x, y, btnW, btnH, bg);
            Display.drawRect(x, y, btnW, btnH, C_MID_GREY);

            Display.setTextSize(1);
            Display.setTextColour(C_WHITE, bg);

            int len = strlen(keys[r][c]);
            int tx = x + (btnW - len * 6) / 2;
            int ty = y + 5;
            Display.setCursor(tx, ty);
            Display.print(keys[r][c]);
        }
    }

    // Legend hints
    Display.drawDivider(136, C_DARK_GREY);
    Display.printCentered("*:Clr/Back  00: .", 138, 1, C_LIGHT_GREY, C_BLACK);

    // Soft-key bar
    Display.drawSoftKeys("*: Clear/Back", "#: Enter", C_NOKIA_BLUE, C_WHITE);
}
