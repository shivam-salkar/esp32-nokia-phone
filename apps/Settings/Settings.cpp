// =============================================================
// Settings.cpp — System Settings App Implementation
// =============================================================

#include "Settings.h"
#include "../../core/DisplayManager.h"
#include "../../core/AppManager.h"
#include "../../storage/StorageManager.h"

const char* Settings::_menuItems[Settings::ITEM_COUNT] = {
    "Display",
    "Theme",
    "Storage",
    "System",
    "About"
};

void Settings::begin() {
    Serial.println(F("[APP] Settings started"));
    _theme      = Storage.getTheme();
    _brightness = Storage.getBrightness();
    _selected   = 0;
    _view       = VIEW_MAIN;
    _drawMain();
}

void Settings::update() {
    // Event-driven
}

void Settings::onInput(InputEvent ev) {
    if (_view == VIEW_MAIN) {
        switch (ev) {
            case INPUT_UP:
            case INPUT_KEY_2:
                if (_selected > 0) {
                    _drawMenuItem(_selected, false);
                    _selected--;
                    _drawMenuItem(_selected, true);
                }
                break;

            case INPUT_DOWN:
            case INPUT_KEY_8:
                if (_selected < ITEM_COUNT - 1) {
                    _drawMenuItem(_selected, false);
                    _selected++;
                    _drawMenuItem(_selected, true);
                }
                break;

            case INPUT_OK:
            case INPUT_KEY_5:
                switch (_selected) {
                    case 0: _view = VIEW_DISPLAY; _drawDisplayView(); break;
                    case 1: _view = VIEW_THEME;   _drawThemeView();   break;
                    case 2: _view = VIEW_STORAGE; _drawStorageView(); break;
                    case 3: _view = VIEW_SYSTEM;  _drawSystemView();  break;
                    case 4: _view = VIEW_ABOUT;   _drawAboutView();   break;
                }
                break;

            case INPUT_BACK:
            case INPUT_STAR:
                AppMgr.launchApp(APP_MENU);
                break;

            case INPUT_HOME:
            case INPUT_KEY_0:
                AppMgr.launchApp(APP_LAUNCHER);
                break;

            default:
                break;
        }
    } else {
        // Sub-views
        if (ev == INPUT_BACK || ev == INPUT_STAR) {
            _view = VIEW_MAIN;
            _drawMain();
            return;
        }

        if (ev == INPUT_HOME || ev == INPUT_KEY_0) {
            AppMgr.launchApp(APP_LAUNCHER);
            return;
        }

        if (_view == VIEW_DISPLAY) {
            if (ev == INPUT_OK || ev == INPUT_KEY_5 || ev == INPUT_RIGHT || ev == INPUT_KEY_6) {
                _brightness = (_brightness % 5) + 1;
                Storage.setBrightness(_brightness);
                _drawDisplayView();
            }
        } else if (_view == VIEW_THEME) {
            if (ev == INPUT_OK || ev == INPUT_KEY_5 || ev == INPUT_RIGHT || ev == INPUT_KEY_6) {
                _theme = (_theme + 1) % 3;
                Storage.setTheme(_theme);
                _drawThemeView();
            }
        }
    }
}

void Settings::_drawMain() {
    Display.clear(C_BLACK);
    Display.drawHeader("SETTINGS", C_NOKIA_BLUE, C_WHITE);

    for (uint8_t i = 0; i < ITEM_COUNT; i++) {
        _drawMenuItem(i, i == _selected);
    }

    Display.drawDivider(136, C_DARK_GREY);
    Display.printCentered("2/8 Nav   5 Select", 138, 1, C_LIGHT_GREY, C_BLACK);
    Display.drawSoftKeys("*: Back", "5: Select", C_NOKIA_BLUE, C_WHITE);
}

void Settings::_drawMenuItem(uint8_t idx, bool hl) {
    int16_t y   = 18 + idx * 23;
    uint16_t bg = hl ? C_NOKIA_BLUE : C_BLACK;
    uint16_t fg = hl ? C_WHITE : C_LIGHT_GREY;

    Display.fillRect(2, y, SCREEN_W - 4, 21, bg);
    if (hl) {
        Display.drawRect(2, y, SCREEN_W - 4, 21, C_NOKIA_CYAN);
    }

    Display.setTextSize(1);
    Display.setTextColour(hl ? C_YELLOW : C_BLACK, bg);
    Display.setCursor(6, y + 6);
    Display.print(hl ? ">" : " ");

    Display.setTextColour(fg, bg);
    Display.setCursor(20, y + 6);
    Display.print(_menuItems[idx]);
}

void Settings::_drawDisplayView() {
    Display.clear(C_BLACK);
    Display.drawHeader("DISPLAY SETTINGS", C_NOKIA_BLUE, C_WHITE);

    Display.setTextSize(1);
    Display.setTextColour(C_WHITE, C_BLACK);
    Display.setCursor(6, 24);
    Display.print("Brightness (Level):");

    // Draw visual brightness bar
    for (uint8_t i = 1; i <= 5; i++) {
        uint16_t col = (i <= _brightness) ? C_YELLOW : C_DARK_GREY;
        Display.fillRect(10 + (i - 1) * 22, 40, 18, 14, col);
        Display.drawRect(10 + (i - 1) * 22, 40, 18, 14, C_WHITE);
    }

    Display.drawDivider(66, C_DARK_GREY);
    Display.setCursor(6, 76);
    Display.print("Screen: 128x160 TFT");
    Display.setCursor(6, 90);
    Display.print("Driver: ST7735");
    Display.setCursor(6, 104);
    Display.print("Orientation: Portrait");

    Display.printCentered("5: Toggle Brightness", 126, 1, C_NOKIA_CYAN, C_BLACK);
    Display.drawSoftKeys("*: Back", "5: Change", C_NOKIA_BLUE, C_WHITE);
}

void Settings::_drawThemeView() {
    Display.clear(C_BLACK);
    Display.drawHeader("THEME SETTINGS", C_NOKIA_BLUE, C_WHITE);

    const char* themeNames[3] = { "Retro Blue", "Dark Matrix", "Classic Light" };
    Display.setTextSize(1);
    Display.setTextColour(C_WHITE, C_BLACK);
    Display.setCursor(6, 26);
    Display.print("Current Theme:");

    Display.fillRect(10, 44, SCREEN_W - 20, 26, C_NOKIA_BLUE);
    Display.drawRect(10, 44, SCREEN_W - 20, 26, C_NOKIA_CYAN);
    Display.printCentered(themeNames[_theme], 52, 1, C_WHITE, C_NOKIA_BLUE);

    Display.printCentered("Press 5 to Switch", 95, 1, C_LIGHT_GREY, C_BLACK);
    Display.drawSoftKeys("*: Back", "5: Switch", C_NOKIA_BLUE, C_WHITE);
}

void Settings::_drawStorageView() {
    Display.clear(C_BLACK);
    Display.drawHeader("STORAGE USAGE", C_NOKIA_BLUE, C_WHITE);

    Display.setTextSize(1);
    Display.setTextColour(C_YELLOW, C_BLACK);
    Display.setCursor(6, 22);
    Display.print("LittleFS Filesystem:");

    size_t total = Storage.totalBytes();
    size_t used  = Storage.usedBytes();
    size_t freeB = (total > used) ? (total - used) : 0;

    Display.setTextColour(C_WHITE, C_BLACK);
    Display.setCursor(6, 38);
    Display.print("Total: ");
    Display.print(String(total / 1024).c_str());
    Display.print(" KB");

    Display.setCursor(6, 52);
    Display.print("Used : ");
    Display.print(String(used / 1024).c_str());
    Display.print(" KB");

    Display.setCursor(6, 66);
    Display.print("Free : ");
    Display.print(String(freeB / 1024).c_str());
    Display.print(" KB");

    // Gauge bar
    Display.drawRect(8, 86, SCREEN_W - 16, 12, C_WHITE);
    int fillW = (total > 0) ? (int)((used * (SCREEN_W - 20)) / total) : 0;
    if (fillW > 0) {
        Display.fillRect(10, 88, fillW, 8, C_NOKIA_GREEN);
    }

    Display.setCursor(6, 110);
    Display.setTextColour(C_LIGHT_GREY, C_BLACK);
    Display.print("Location: /notes");

    Display.drawSoftKeys("*: Back", "0: Home", C_NOKIA_BLUE, C_WHITE);
}

void Settings::_drawSystemView() {
    Display.clear(C_BLACK);
    Display.drawHeader("SYSTEM INFO", C_NOKIA_BLUE, C_WHITE);

    Display.setTextSize(1);
    Display.setTextColour(C_YELLOW, C_BLACK);
    Display.setCursor(6, 20);
    Display.print("ESP32-S3 System:");

    Display.setTextColour(C_WHITE, C_BLACK);
    Display.setCursor(6, 34);
    Display.print("Chip Rev: ");
    Display.print(String(ESP.getChipRevision()).c_str());

    Display.setCursor(6, 48);
    Display.print("CPU Freq: ");
    Display.print(String(ESP.getCpuFreqMHz()).c_str());
    Display.print(" MHz");

    Display.setCursor(6, 62);
    Display.print("Flash: ");
    Display.print(String(ESP.getFlashChipSize() / (1024 * 1024)).c_str());
    Display.print(" MB");

    Display.setCursor(6, 76);
    Display.print("PSRAM: ");
    Display.print(String(ESP.getPsramSize() / (1024 * 1024)).c_str());
    Display.print(" MB");

    Display.setCursor(6, 90);
    Display.print("Free Heap: ");
    Display.print(String(ESP.getFreeHeap() / 1024).c_str());
    Display.print(" KB");

    Display.setCursor(6, 104);
    Display.print("Min Free: ");
    Display.print(String(ESP.getMinFreeHeap() / 1024).c_str());
    Display.print(" KB");

    Display.drawSoftKeys("*: Back", "0: Home", C_NOKIA_BLUE, C_WHITE);
}

void Settings::_drawAboutView() {
    Display.clear(C_BLACK);
    Display.drawHeader("ABOUT OS", C_NOKIA_BLUE, C_WHITE);

    Display.setTextSize(1);
    Display.setTextColour(C_NOKIA_CYAN, C_BLACK);
    Display.setCursor(6, 22);
    Display.print("ESP32 Mini OS");

    Display.setTextColour(C_LIGHT_GREY, C_BLACK);
    Display.setCursor(6, 36);
    Display.print("Version 1.0 (Final)");

    Display.drawDivider(50, C_DARK_GREY);
    Display.setTextColour(C_WHITE, C_BLACK);
    Display.setCursor(6, 58);
    Display.print("Framework:");
    Display.setCursor(6, 70);
    Display.print("- Arduino-ESP32");

    Display.setCursor(6, 88);
    Display.print("Hardware:");
    Display.setCursor(6, 100);
    Display.print("- ESP32-S3 16MB/8MB");
    Display.setCursor(6, 112);
    Display.print("- ST7735 128x160 TFT");
    Display.setCursor(6, 124);
    Display.print("- 4x4 Membrane Keypad");

    Display.drawSoftKeys("*: Back", "0: Home", C_NOKIA_BLUE, C_WHITE);
}
