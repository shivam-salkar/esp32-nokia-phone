// =============================================================
// AppManager.cpp — Application Lifecycle Controller
// =============================================================

#include "AppManager.h"
#include "InputManager.h"
#include "DisplayManager.h"

#include "../apps/Calculator/Calculator.h"
#include "../apps/TextEditor/TextEditor.h"
#include "../apps/Settings/Settings.h"
#include "../apps/Snake/Snake.h"
#include "../apps/About/About.h"

#include "../ui/Launcher.h"
#include "../ui/Menu.h"

// Singleton
AppManager AppMgr;

void AppManager::begin() {
    _currentApp = nullptr;
    _currentID  = APP_LAUNCHER;
    Serial.println(F("[APP] AppManager ready"));
    launchApp(APP_LAUNCHER);
}

void AppManager::update() {
    if (Input.hasEvent() && _currentApp) {
        InputEvent ev = Input.getEvent();
        _currentApp->onInput(ev);
    }

    if (_currentApp) {
        _currentApp->update();
    }
}

void AppManager::launchApp(AppID id) {
    Serial.print(F("[APP] Launching app ID: "));
    Serial.println((int)id);

    // Default to navigation mode whenever switching apps
    Input.setMode(INPUT_MODE_NAV);

    _destroyCurrentApp();

    _currentID  = id;
    _currentApp = _createApp(id);

    if (_currentApp) {
        _currentApp->begin();
    }
}

App* AppManager::_createApp(AppID id) {
    switch (id) {
        case APP_LAUNCHER:    return new Launcher();
        case APP_MENU:        return new Menu();
        case APP_CALCULATOR:  return new Calculator();
        case APP_NOTES:       return new TextEditor();
        case APP_SETTINGS:    return new Settings();
        case APP_SNAKE:       return new Snake();
        case APP_ABOUT:       return new About();
        default:
            Serial.println(F("[APP] Unknown AppID — fallback to Launcher"));
            return new Launcher();
    }
}

void AppManager::_destroyCurrentApp() {
    if (_currentApp) {
        delete _currentApp;
        _currentApp = nullptr;
    }
}
