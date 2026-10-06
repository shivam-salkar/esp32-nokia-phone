// =============================================================
// Calculator.h — 4-function Calculator App
// =============================================================

#ifndef CALCULATOR_H
#define CALCULATOR_H

#include "../../core/AppManager.h"
#include "../../core/InputManager.h"

class Calculator : public App {
public:
    void begin()  override;
    void update() override;
    void onInput(InputEvent ev) override;

private:
    // Expression state
    double  _operandA;
    double  _operandB;
    char    _operator;       // '+' '-' '*' '/'
    bool    _enteringB;      // false=entering A, true=entering B
    bool    _showResult;
    bool    _errorState;

    // Current digit string being entered
    char    _input[16];
    uint8_t _inputLen;

    // Soft cursor — which digit/symbol is highlighted on keypad
    uint8_t _cursor;         // 0-15 maps to keypad layout

    void _draw();
    void _drawDisplay();
    void _drawKeypad();
    void _highlightKey(uint8_t idx, bool on);
    void _pressKey(uint8_t idx);
    void _calculate();
    void _clearInput();

    // Keypad layout: 4 rows × 4 cols
    static const char* _keyLabels[16];
};

#endif // CALCULATOR_H
