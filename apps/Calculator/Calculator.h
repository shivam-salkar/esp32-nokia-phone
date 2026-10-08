// =============================================================
// Calculator.h — 4-function Floating-Point Calculator App
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
    double   _operandA;
    double   _operandB;
    char     _operator;       // '+' '-' '*' '/'
    bool     _enteringB;
    bool     _showResult;
    bool     _errorState;

    char     _input[20];
    uint8_t  _inputLen;

    uint32_t _lastZeroPress;  // For double-tap '0' -> '.' detection

    void _draw();
    void _drawDisplay();
    void _drawKeypadHelp();
    void _appendDigit(char digit);
    void _appendDecimal();
    void _setOperator(char op);
    void _calculate();
    void _clearAll();
    void _backspace();
};

#endif // CALCULATOR_H
