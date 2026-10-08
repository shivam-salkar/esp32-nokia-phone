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
    void _drawClock(bool colonBlink);
    void _drawStatusBar();

    uint32_t _lastClockUpdate;
    uint32_t _bootSeconds;
    bool     _colonBlink;
};

#endif // LAUNCHER_H
