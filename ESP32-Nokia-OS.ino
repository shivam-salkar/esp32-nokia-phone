// =============================================================
// ESP32-Nokia-OS.ino — Main Entry Point
// =============================================================
// Arduino IDE project for ESP32-S3 + 1.8" ST7735 TFT (128×160)
// + 4×4 Membrane Keypad
//
// Required libraries (Arduino Library Manager):
//   - Adafruit GFX Library
//   - Adafruit ST7735 and ST7789 Library
//   - Keypad Library (by Mark Stanley, Alexander Brevig)
//
// Board: ESP32S3 Dev Module
// Flash Size: 16MB (or 4MB/8MB)
// PSRAM: OPI PSRAM (or enabled)
// Partition Scheme: Default with spiffs / LittleFS
//
// Serial Monitor: 115200 baud, Line ending: Newline (\n)
//
// Navigation (Keypad):
//   2 → UP        8 → DOWN
//   4 → LEFT      6 → RIGHT
//   5 → OK / SEL  * → BACK
//   # → MENU      0 → HOME
//   1..9,0        → Numeric / Multi-tap in apps
//   A, B, C, D    → App-specific actions
// =============================================================

// ─────────────────────────────────────────────────────────────
// Headers
// ─────────────────────────────────────────────────────────────
#include "config/pins.h"
#include "storage/StorageManager.h"
#include "core/OS.h"
#include "core/DisplayManager.h"
#include "core/InputManager.h"
#include "core/AppManager.h"

// ─────────────────────────────────────────────────────────────
// Arduino IDE does NOT auto-compile .cpp files inside sketch
// sub-folders. We must #include each one explicitly here so
// the IDE passes them to the compiler.
// ─────────────────────────────────────────────────────────────
#include "storage/StorageManager.cpp"
#include "core/OS.cpp"
#include "core/DisplayManager.cpp"
#include "core/InputManager.cpp"
#include "core/AppManager.cpp"
#include "ui/Launcher.cpp"
#include "ui/Menu.cpp"
#include "apps/Calculator/Calculator.cpp"
#include "apps/TextEditor/TextEditor.cpp"
#include "apps/Settings/Settings.cpp"
#include "apps/Snake/Snake.cpp"
#include "apps/About/About.cpp"

// ─────────────────────────────────────────────────────────────
void setup() {
    Serial.begin(115200);
    delay(400);   // Allow USB-serial CDC to enumerate

    NokiaOS.begin();
}

// ─────────────────────────────────────────────────────────────
void loop() {
    NokiaOS.update();
}
