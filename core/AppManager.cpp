// =============================================================
// AppManager.cpp — Application Lifecycle Controller
// =============================================================

#include "AppManager.h"
#include "InputManager.h"
#include "DisplayManager.h"

// Forward-declare app constructors (avoids circular includes)
#include "../apps/Calculator/Calculator.h"
#include "../apps/TextEditor/TextEditor.h"
#include "../apps/Settings/Settings.h"
#include "../apps/Snake/Snake.h"

// Launcher and Menu are defined in ui/
#include "../ui/Launcher.h"
#include "../ui/Menu.h"

// Singleton
AppManager AppMgr;

// ─────────────────────────────────────────────────────────────
void AppManager::begin() {
    _currentApp = nullptr;
    _currentID  = APP_LAUNCHER;
    Serial.println(F("[APP] AppManager ready"));
    launchApp(APP_LAUNCHER);
}

// ─────────────────────────────────────────────────────────────
void AppManager::update() {
    // Deliver any pending input to the active app
    if (Input.hasEvent() && _currentApp) {
        InputEvent ev = Input.getEvent();
        _currentApp->onInput(ev);
    }

    // Tick the active app
    if (_currentApp) {
        _currentApp->update();
    }
}

// ─────────────────────────────────────────────────────────────
void AppManager::launchApp(AppID id) {
    Serial.print(F("[APP] Launching app ID: "));
    Serial.println((int)id);

    _destroyCurrentApp();

    _currentID  = id;
    _currentApp = _createApp(id);

    if (_currentApp) {
        _currentApp->begin();
    }
}

// ─────────────────────────────────────────────────────────────
App* AppManager::_createApp(AppID id) {
    switch (id) {
        case APP_LAUNCHER:    return new Launcher();
        case APP_MENU:        return new Menu();
        case APP_CALCULATOR:  return new Calculator();
        case APP_TEXT_EDITOR: return new TextEditor();
        case APP_SETTINGS:    return new Settings();
        case APP_SNAKE:       return new Snake();
        default:
            Serial.println(F("[APP] Unknown AppID — falling back to Launcher"));
            return new Launcher();
    }
}

// ─────────────────────────────────────────────────────────────
void AppManager::_destroyCurrentApp() {
    if (_currentApp) {
        delete _currentApp;
        _currentApp = nullptr;
    }
}
