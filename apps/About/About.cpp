// =============================================================
// About.cpp — About System Information App
// =============================================================

#include "About.h"
#include "../../core/DisplayManager.h"
#include "../../core/AppManager.h"

void About::begin() {
    Serial.println(F("[APP] About opened"));
    _draw();
}

void About::update() {
    // Event driven
}

void About::onInput(InputEvent ev) {
    if (ev == INPUT_BACK || ev == INPUT_OK || ev == INPUT_STAR) {
        AppMgr.launchApp(APP_MENU);
    } else if (ev == INPUT_HOME || ev == INPUT_KEY_0) {
        AppMgr.launchApp(APP_LAUNCHER);
    }
}

void About::_draw() {
    Display.clear(C_BLACK);
    Display.drawHeader("ABOUT SYSTEM", C_NOKIA_BLUE, C_WHITE);

    Display.setTextSize(1);

    // OS Title
    Display.setTextColour(C_NOKIA_CYAN, C_BLACK);
    Display.setCursor(4, 18);
    Display.print("ESP32 Mini OS v1.0");

    Display.drawDivider(28, C_DARK_GREY);

    // Hardware
    Display.setTextColour(C_YELLOW, C_BLACK);
    Display.setCursor(4, 32);
    Display.print("Hardware:");
    Display.setTextColour(C_WHITE, C_BLACK);
    Display.setCursor(4, 42);
    Display.print("- ESP32-S3 DevKit");
    Display.setCursor(4, 52);
    Display.print("- 1.8\" TFT 128x160");
    Display.setCursor(4, 62);
    Display.print("- 4x4 Membrane Key");

    Display.drawDivider(74, C_DARK_GREY);

    // Software
    Display.setTextColour(C_YELLOW, C_BLACK);
    Display.setCursor(4, 78);
    Display.print("Software:");
    Display.setTextColour(C_WHITE, C_BLACK);
    Display.setCursor(4, 88);
    Display.print("- Arduino-ESP32");

    Display.drawDivider(100, C_DARK_GREY);

    // Dynamic metrics
    Display.setTextColour(C_YELLOW, C_BLACK);
    Display.setCursor(4, 104);
    Display.print("Memory (Live):");

    Display.setTextColour(C_WHITE, C_BLACK);
    Display.setCursor(4, 114);
    Display.print("Flash: ");
    Display.print(String(ESP.getFlashChipSize() / (1024 * 1024)).c_str());
    Display.print(" MB");

    Display.setCursor(4, 124);
    Display.print("PSRAM: ");
    Display.print(String(ESP.getPsramSize() / (1024 * 1024)).c_str());
    Display.print(" MB");

    Display.setCursor(4, 134);
    Display.print("Free Heap: ");
    Display.print(String(ESP.getFreeHeap() / 1024).c_str());
    Display.print(" KB");

    // Softkey footer
    Display.drawSoftKeys("*:Back", "0:Home", C_NOKIA_BLUE, C_WHITE);
}
