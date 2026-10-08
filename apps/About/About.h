// =============================================================
// About.h — About System Information App
// =============================================================

#ifndef ABOUT_H
#define ABOUT_H

#include "../../core/AppManager.h"
#include "../../core/InputManager.h"

class About : public App {
public:
    void begin()  override;
    void update() override;
    void onInput(InputEvent ev) override;

private:
    void _draw();
};

#endif // ABOUT_H
