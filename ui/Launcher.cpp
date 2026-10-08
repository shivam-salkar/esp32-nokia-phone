// =============================================================
// Launcher.cpp — Home Screen Implementation
// =============================================================

#include "Launcher.h"
#include "../core/DisplayManager.h"
#include "../core/AppManager.h"

void Launcher::begin() {
    Serial.println(F("[APP] Home Screen started"));
    _lastClockUpdate = millis();
    _bootSeconds     = millis() / 1000;
    _colonBlink      = true;
    _draw();
}

void Launcher::update() {
    uint32_t now = millis();
    if (now - _lastClockUpdate >= 1000) {
        _lastClockUpdate = now;
        _bootSeconds++;
        _colonBlink = !_colonBlink;
        _drawClock(_colonBlink);
    }
}

void Launcher::onInput(InputEvent ev) {
    switch (ev) {
        case INPUT_MENU:
        case INPUT_OK:
        case INPUT_KEY_5:
        case INPUT_HASH:
            AppMgr.launchApp(APP_MENU);
            break;

        case INPUT_HOME:
        case INPUT_KEY_0:
            // Already Home — trigger a fresh redraw
            _draw();
            break;

        default:
            break;
    }
}

void Launcher::_drawStatusBar() {
    Display.fillRect(0, 0, SCREEN_W, 14, C_NOKIA_BLUE);
    Display.setTextColour(C_WHITE, C_NOKIA_BLUE);
    Display.setTextSize(1);

    // Current time approximation (HH:MM based on uptime offset from 10:42)
    uint32_t totalSec = (10 * 3600 + 42 * 60) + _bootSeconds;
    uint8_t h = (totalSec / 3600) % 24;
    uint8_t m = (totalSec / 60) % 60;

    char timeStr[8];
    snprintf(timeStr, sizeof(timeStr), "%02u:%02u", h, m);
    Display.setCursor(3, 3);
    Display.print(timeStr);

    // Wi-Fi status indicator (Honest status: WiFi hardware present, not connected)
    Display.setCursor(SCREEN_W - 54, 3);
    Display.print("WiFi:Off");

    Display.drawDivider(14, C_DARK_GREY);
}

void Launcher::_draw() {
    Display.clear(C_BLACK);
    _drawStatusBar();

    // Decorative retro Nokia double border
    Display.drawRect(4, 18, SCREEN_W - 8, SCREEN_H - 34, C_NOKIA_BLUE);
    Display.drawRect(6, 20, SCREEN_W - 12, SCREEN_H - 38, C_DARK_GREY);

    // Title badge
    Display.fillRect(16, 26, SCREEN_W - 32, 16, C_NOKIA_BLUE);
    Display.printCentered("ESP32 OS", 30, 1, C_WHITE, C_NOKIA_BLUE);

    // Retro phone / smiley symbol in center
    int cx = SCREEN_W / 2;
    int cy = 72;
    Display.drawCircle(cx, cy, 14, C_YELLOW);
    // Eyes
    Display.drawPixel(cx - 5, cy - 4, C_YELLOW);
    Display.drawPixel(cx - 4, cy - 4, C_YELLOW);
    Display.drawPixel(cx + 4, cy - 4, C_YELLOW);
    Display.drawPixel(cx + 5, cy - 4, C_YELLOW);
    // Smile
    Display.drawPixel(cx - 6, cy + 3, C_YELLOW);
    Display.drawPixel(cx - 5, cy + 5, C_YELLOW);
    Display.drawFastHLine(cx - 4, cy + 6, 9, C_YELLOW);
    Display.drawPixel(cx + 5, cy + 5, C_YELLOW);
    Display.drawPixel(cx + 6, cy + 3, C_YELLOW);

    // Center clock
    _drawClock(true);

    // Help text
    Display.printCentered("Press # for Menu", 124, 1, C_LIGHT_GREY, C_BLACK);

    // Soft-key bar
    Display.drawSoftKeys("#: MENU", "5: OK", C_NOKIA_BLUE, C_WHITE);
}

void Launcher::_drawClock(bool colonBlink) {
    uint32_t totalSec = (10 * 3600 + 42 * 60) + _bootSeconds;
    uint8_t h = (totalSec / 3600) % 24;
    uint8_t m = (totalSec / 60) % 60;
    uint8_t s = totalSec % 60;

    // Clear clock sub-area only (avoids full-screen flicker)
    Display.fillRect(10, 96, SCREEN_W - 20, 20, C_BLACK);

    char buf[12];
    snprintf(buf, sizeof(buf), "%02u%c%02u%c%02u",
             h, colonBlink ? ':' : ' ',
             m, colonBlink ? ':' : ' ',
             s);

    Display.printCentered(buf, 99, 2, C_NOKIA_CYAN, C_BLACK);
}
