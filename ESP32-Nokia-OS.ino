// =============================================================
// ESP32-Nokia-OS.ino — Main Entry Point
// =============================================================
// Arduino IDE project for ESP32-S3 + 1.8" ST7735 TFT (128×160)
//
// Required libraries (Arduino Library Manager):
//   - Adafruit GFX Library
//   - Adafruit ST7735 and ST7789 Library
//
// Board: ESP32S3 Dev Module  (or equivalent ESP32-S3 board)
// Flash Mode: QIO
// Flash Size: 4MB (or match your board)
// Partition Scheme: Default 4MB with spiffs
//   → or "Default 4MB with ffat"  (LittleFS uses SPIFFS partition)
//
// Serial Monitor: 115200 baud, Line ending: Newline (\n)
//
// FIRST: Upload TFT_Test/TFT_Test.ino and verify display works.
//
// Input commands via Serial Monitor:
//   UP / u       DOWN / d     LEFT / l
//   RIGHT / r    OK / o       BACK / b
//   MENU / m
// =============================================================

// ─────────────────────────────────────────────────────────────
// Headers
// ─────────────────────────────────────────────────────────────
#include "core/OS.h"
#include "core/DisplayManager.h"
#include "core/InputManager.h"
#include "core/AppManager.h"

// ─────────────────────────────────────────────────────────────
// Arduino IDE does NOT auto-compile .cpp files inside sketch
// sub-folders. We must #include each one explicitly here so
// the IDE passes them to the compiler.
// ─────────────────────────────────────────────────────────────
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

// ─────────────────────────────────────────────────────────────
void setup() {
    Serial.begin(115200);
    delay(300);   // Let USB-serial enumerate

    NokiaOS.begin();
}

// ─────────────────────────────────────────────────────────────
void loop() {
    NokiaOS.update();
}
