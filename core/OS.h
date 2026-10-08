// =============================================================
// OS.h — Top-level OS coordinator
// =============================================================

#ifndef OS_H
#define OS_H

#include <Arduino.h>

class OS {
public:
    void begin();   // Call once in Arduino setup()
    void update();  // Call every Arduino loop() iteration
};

extern OS NokiaOS;

#endif // OS_H
