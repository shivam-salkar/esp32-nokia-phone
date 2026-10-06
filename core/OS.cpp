// =============================================================
// OS.cpp — Top-level OS coordinator
// =============================================================

#include "OS.h"
#include "DisplayManager.h"
#include "InputManager.h"
#include "AppManager.h"

OS NokiaOS;

// ─────────────────────────────────────────────────────────────
void OS::begin() {
    Serial.println(F("======================"));
    Serial.println(F("  ESP32-Nokia-OS Boot "));
    Serial.println(F("======================"));
    Serial.println(F("[BOOT] Starting OS"));

    Display.begin();
    Input.begin();
    AppMgr.begin();

    Serial.println(F("[BOOT] OS ready"));
}

// ─────────────────────────────────────────────────────────────
void OS::update() {
    Input.update();
    AppMgr.update();
}
