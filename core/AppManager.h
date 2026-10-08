// =============================================================
// AppManager.h — Application Lifecycle Controller
// =============================================================

#ifndef APP_MANAGER_H
#define APP_MANAGER_H

#include <Arduino.h>
#include "InputManager.h"

// ─────────────────────────────────────────────────────────────
// App identifiers
// ─────────────────────────────────────────────────────────────
enum AppID {
    APP_LAUNCHER,
    APP_MENU,
    APP_CALCULATOR,
    APP_NOTES,
    APP_SNAKE,
    APP_SETTINGS,
    APP_ABOUT
};

// ─────────────────────────────────────────────────────────────
// Base App interface
// ─────────────────────────────────────────────────────────────
class App {
public:
    virtual ~App() {}
    virtual void begin()  = 0;
    virtual void update() = 0;
    virtual void onInput(InputEvent ev) = 0;
};

// ─────────────────────────────────────────────────────────────
// AppManager
// ─────────────────────────────────────────────────────────────
class AppManager {
public:
    void begin();
    void update();
    void launchApp(AppID id);
    AppID currentAppID() const { return _currentID; }

private:
    App*  _currentApp;
    AppID _currentID;

    App* _createApp(AppID id);
    void _destroyCurrentApp();
};

extern AppManager AppMgr;

#endif // APP_MANAGER_H
