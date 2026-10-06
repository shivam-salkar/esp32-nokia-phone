// =============================================================
// Settings.cpp — System Settings App
// =============================================================
// Items:
//   Theme        → cycle Dark / Light
//   Brightness   → 1–5 bars (visual only — no PWM pin yet)
//   Storage Info → LittleFS free/total
//   About        → firmware info
//
// Controls:
//   UP/DOWN → move
//   OK      → enter / toggle
//   BACK    → return to menu
// =============================================================

#include "Settings.h"
#include "../../core/DisplayManager.h"
#include "../../core/AppManager.h"
#include <LittleFS.h>

const char* Settings::_labels[Settings::ITEM_COUNT] = {
    "Theme",
    "Brightness",
    "Storage",
    "About"
};

// ─────────────────────────────────────────────────────────────
void Settings::begin() {
    Serial.println(F("[APP] Settings started"));
    _selected = 0;
    _mode     = MODE_LIST;
    _load();
    _drawList();
}

// ─────────────────────────────────────────────────────────────
void Settings::update() { /* no ticks needed */ }

// ─────────────────────────────────────────────────────────────
void Settings::onInput(InputEvent ev) {
    switch (_mode) {

        case MODE_LIST:
            if (ev == INPUT_UP   && _selected > 0) {
                _selected--; _drawList();
            } else if (ev == INPUT_DOWN && _selected < ITEM_COUNT - 1) {
                _selected++; _drawList();
            } else if (ev == INPUT_OK) {
                switch (_selected) {
                    case 0: _mode = MODE_THEME; _drawTheme(); break;
                    case 1:
                        // Toggle brightness in list view
                        _brightness = (_brightness + 1) % 5;
                        _save();
                        _drawList();
                        break;
                    case 2:
                        // Storage info — show inline
                        _drawList(); // redraws with fs info
                        break;
                    case 3: _mode = MODE_ABOUT; _drawAbout(); break;
                }
            } else if (ev == INPUT_BACK) {
                AppMgr.launchApp(APP_MENU);
            }
            break;

        case MODE_THEME:
            if (ev == INPUT_LEFT || ev == INPUT_RIGHT) {
                _theme = (_theme + 1) % 2;
                _save();
                _drawTheme();
            } else if (ev == INPUT_BACK || ev == INPUT_OK) {
                _mode = MODE_LIST;
                _drawList();
            }
            break;

        case MODE_ABOUT:
            if (ev == INPUT_BACK || ev == INPUT_OK) {
                _mode = MODE_LIST;
                _drawList();
            }
            break;
    }
}

// ─────────────────────────────────────────────────────────────
void Settings::_load() {
    _prefs.begin("settings", false);
    _theme      = _prefs.getUChar("theme",  0);
    _brightness = _prefs.getUChar("bright", 3);
    _prefs.end();
    Serial.print(F("[SETTINGS] Loaded theme="));
    Serial.print(_theme);
    Serial.print(F(" bright="));
    Serial.println(_brightness);
}

void Settings::_save() {
    _prefs.begin("settings", false);
    _prefs.putUChar("theme",  _theme);
    _prefs.putUChar("bright", _brightness);
    _prefs.end();
    Serial.println(F("[SETTINGS] Saved"));
}

// ─────────────────────────────────────────────────────────────
const char* Settings::_themeLabel() {
    return _theme == 0 ? "Dark" : "Light";
}

// ─────────────────────────────────────────────────────────────
void Settings::_drawList() {
    Display.clear(C_BLACK);
    Display.drawHeader("Settings", C_NOKIA_BLUE, C_WHITE);
    Display.drawDivider(16, C_DARK_GREY);

    for (uint8_t i = 0; i < ITEM_COUNT; i++) {
        _drawItem(i, i == _selected);
    }

    // Brightness bar at bottom of list
    Display.fillRect(0, 148, SCREEN_W, 12, C_DARK_GREY);
    Display.setTextColour(C_WHITE, C_DARK_GREY);
    Display.setTextSize(1);
    Display.setCursor(2, 150);
    Display.print("OK=Select");
    Display.setCursor(SCREEN_W - 34, 150);
    Display.print("B=Back");
}

// ─────────────────────────────────────────────────────────────
void Settings::_drawItem(uint8_t idx, bool hl) {
    int16_t  y  = 20 + idx * 28;
    uint16_t bg = hl ? C_NOKIA_BLUE : C_BLACK;

    Display.fillRect(0, y, SCREEN_W, 26, bg);
    Display.setTextSize(1);
    Display.setTextColour(C_WHITE, bg);
    Display.setCursor(4, y + 4);
    Display.print(hl ? "> " : "  ");
    Display.print(_labels[idx]);

    // Show current value for theme / brightness
    Display.setTextColour(C_NOKIA_GREEN, bg);
    if (idx == 0) {
        Display.setCursor(SCREEN_W - strlen(_themeLabel()) * 6 - 4, y + 4);
        Display.print(_themeLabel());
    } else if (idx == 1) {
        // Brightness bars
        for (uint8_t b = 0; b < 5; b++) {
            uint16_t bc = (b <= _brightness) ? C_NOKIA_GREEN : C_DARK_GREY;
            Display.fillRect(SCREEN_W - 34 + b * 6, y + 5, 4, 8, bc);
        }
    } else if (idx == 2) {
        // Storage
        if (LittleFS.begin(false)) {
            char buf[16];
            size_t total = LittleFS.totalBytes();
            size_t used  = LittleFS.usedBytes();
            snprintf(buf, sizeof(buf), "%dK/%dK", (int)(used/1024), (int)(total/1024));
            Display.setCursor(SCREEN_W - strlen(buf) * 6 - 4, y + 4);
            Display.print(buf);
        }
    }
}

// ─────────────────────────────────────────────────────────────
void Settings::_drawTheme() {
    Display.clear(C_BLACK);
    Display.drawHeader("Theme", C_NOKIA_BLUE, C_WHITE);
    Display.drawDivider(16, C_DARK_GREY);

    Display.printCentered("< Press L/R >", 50, 1, C_LIGHT_GREY, C_BLACK);
    Display.printCentered(_themeLabel(), 70, 2, C_NOKIA_GREEN, C_BLACK);
    Display.printCentered("OK=Confirm", 100, 1, C_DARK_GREY, C_BLACK);
}

// ─────────────────────────────────────────────────────────────
void Settings::_drawAbout() {
    Display.clear(C_BLACK);
    Display.drawHeader("About", C_NOKIA_BLUE, C_WHITE);
    Display.drawDivider(16, C_DARK_GREY);

    Display.printCentered("ESP32-Nokia-OS", 25, 1, C_WHITE,       C_BLACK);
    Display.printCentered("v1.0.0",         38, 1, C_NOKIA_GREEN, C_BLACK);
    Display.drawDivider(50, C_DARK_GREY);
    Display.printCentered("Board: ESP32-S3", 56, 1, C_LIGHT_GREY, C_BLACK);
    Display.printCentered("TFT: ST7735",     68, 1, C_LIGHT_GREY, C_BLACK);
    Display.printCentered("128x160 px",      80, 1, C_LIGHT_GREY, C_BLACK);
    Display.drawDivider(92, C_DARK_GREY);
    Display.printCentered("Input: Serial",   98, 1, C_LIGHT_GREY, C_BLACK);
    Display.printCentered("Storage: LittleFS",110,1, C_LIGHT_GREY, C_BLACK);

    Display.fillRect(0, 148, SCREEN_W, 12, C_DARK_GREY);
    Display.setTextColour(C_WHITE, C_DARK_GREY);
    Display.setTextSize(1);
    Display.setCursor(SCREEN_W / 2 - 15, 150);
    Display.print("BACK=Exit");
}
