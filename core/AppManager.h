// =============================================================
// AppManager.h — Application Lifecycle Controller
// =============================================================
// Manages the currently active app.
// Apps are identified by an AppID enum.
// The manager calls begin(), update(), and onInput() on the
// active app — no app logic lives in the main sketch loop.
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
    APP_TEXT_EDITOR,
    APP_SETTINGS,
    APP_SNAKE
};

// ─────────────────────────────────────────────────────────────
// Base App interface — every app inherits from this
// ─────────────────────────────────────────────────────────────
class App {
public:
    virtual ~App() {}
    virtual void begin()  = 0;               // Called once on app start
    virtual void update() = 0;               // Called every loop tick
    virtual void onInput(InputEvent ev) = 0; // Called when an event arrives
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

// Singleton
extern AppManager AppMgr;

#endif // APP_MANAGER_H
