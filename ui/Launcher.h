// =============================================================
// Launcher.h — Home Screen / Launcher App
// =============================================================

#ifndef LAUNCHER_H
#define LAUNCHER_H

#include "../core/AppManager.h"
#include "../core/InputManager.h"

class Launcher : public App {
public:
    void begin()  override;
    void update() override;
    void onInput(InputEvent ev) override;

private:
    void _draw();
    void _drawClock();

    uint32_t _lastClockUpdate;
    uint8_t  _fakeSeconds;
    uint8_t  _fakeMinutes;
    uint8_t  _fakeHours;
};

#endif // LAUNCHER_H
