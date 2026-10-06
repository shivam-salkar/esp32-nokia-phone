// =============================================================
// Launcher.cpp — Home Screen
// =============================================================
// Layout (128×160):
//   Row 0-15   : Status bar  (time left, "OS" right)
//   Row 16     : Divider
//   Row 17-100 : Main area   (clock, title)
//   Row 101-149: Hint bar
//   Row 150-159: Bottom bar  (soft-key hints)
//
// Controls:
//   OK / MENU → open main menu
// =============================================================

#include "Launcher.h"
#include "../core/DisplayManager.h"
#include "../core/AppManager.h"

// ─────────────────────────────────────────────────────────────
void Launcher::begin() {
    Serial.println(F("[APP] Launcher started"));
    _fakeHours   = 12;
    _fakeMinutes = 0;
    _fakeSeconds = 0;
    _lastClockUpdate = millis();
    _draw();
}

// ─────────────────────────────────────────────────────────────
void Launcher::update() {
    // Advance fake clock every second
    uint32_t now = millis();
    if (now - _lastClockUpdate >= 1000) {
        _lastClockUpdate = now;
        _fakeSeconds++;
        if (_fakeSeconds >= 60) { _fakeSeconds = 0; _fakeMinutes++; }
        if (_fakeMinutes >= 60) { _fakeMinutes = 0; _fakeHours++;   }
        if (_fakeHours   >= 24)   _fakeHours   = 0;
        _drawClock();
    }
}

// ─────────────────────────────────────────────────────────────
void Launcher::onInput(InputEvent ev) {
    if (ev == INPUT_OK || ev == INPUT_MENU) {
        AppMgr.launchApp(APP_MENU);
    }
}

// ─────────────────────────────────────────────────────────────
void Launcher::_draw() {
    Display.clear(C_BLACK);

    // ── Status bar ───────────────────────────────────────
    Display.fillRect(0, 0, SCREEN_W, 14, C_NOKIA_BLUE);
    Display.setTextColour(C_WHITE, C_NOKIA_BLUE);
    Display.setTextSize(1);

    // "ESP32" left side
    Display.setCursor(2, 3);
    Display.print("ESP32");

    // Signal bars right side (static decoration)
    for (uint8_t i = 0; i < 4; i++) {
        Display.fillRect(SCREEN_W - 20 + i * 5, 3 + (3 - i) * 2,
                         3, 2 + i * 2, C_NOKIA_GREEN);
    }

    // ── Divider ──────────────────────────────────────────
    Display.drawDivider(14, C_DARK_GREY);

    // ── Title ────────────────────────────────────────────
    Display.printCentered("Nokia OS", 30, 2, C_NOKIA_GREEN, C_BLACK);
    Display.drawDivider(50, C_DARK_GREY);

    // ── Clock ────────────────────────────────────────────
    _drawClock();

    // ── Decorative border ────────────────────────────────
    Display.drawRect(4, 26, SCREEN_W - 8, 95, C_NOKIA_BLUE);

    // ── Hint ─────────────────────────────────────────────
    Display.printCentered("Press OK for Menu", 126, 1, C_LIGHT_GREY, C_BLACK);

    // ── Bottom soft-key bar ──────────────────────────────
    Display.fillRect(0, 148, SCREEN_W, 12, C_DARK_GREY);
    Display.setTextColour(C_WHITE, C_DARK_GREY);
    Display.setTextSize(1);
    Display.setCursor(4, 150);
    Display.print("MENU");
    Display.setCursor(SCREEN_W - 28, 150);
    Display.print("BACK");
}

// ─────────────────────────────────────────────────────────────
void Launcher::_drawClock() {
    // Clear clock area
    Display.fillRect(10, 60, SCREEN_W - 20, 40, C_BLACK);

    // Build time string HH:MM:SS
    char buf[12];
    snprintf(buf, sizeof(buf), "%02d:%02d:%02d",
             _fakeHours, _fakeMinutes, _fakeSeconds);

    Display.printCentered(buf, 68, 2, C_WHITE, C_BLACK);

    // Day label
    Display.printCentered("Mon 06 Oct", 90, 1, C_LIGHT_GREY, C_BLACK);
}
