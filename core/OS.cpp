// =============================================================
// OS.cpp — Top-level OS coordinator
// =============================================================

#include "OS.h"
#include "DisplayManager.h"
#include "InputManager.h"
#include "AppManager.h"
#include "../storage/StorageManager.h"

OS NokiaOS;

// ─────────────────────────────────────────────────────────────
void OS::begin() {
    Serial.println(F("[BOOT] ESP32 Mini OS"));

    Display.begin();
    Display.showBootScreen();

    Input.begin();
    Storage.begin();

    Serial.println(F("[OS] Starting Home Screen"));
    AppMgr.begin();
}

// ─────────────────────────────────────────────────────────────
void OS::update() {
    Input.update();
    AppMgr.update();
}
