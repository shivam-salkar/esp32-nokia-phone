// =============================================================
// Menu.cpp — Main Application Menu
// =============================================================
// Layout (128×160):
//   Row 0-15  : Header "MENU"
//   Row 16    : Divider
//   Row 17+   : Menu items (22px each)
//   Bottom    : Soft-key bar
//
// Controls:
//   UP   → move selection up
//   DOWN → move selection down
//   OK   → open selected app
//   BACK → return to Launcher
// =============================================================

#include "Menu.h"
#include "../core/DisplayManager.h"
#include "../core/AppManager.h"

// ─────────────────────────────────────────────────────────────
// Static menu items
// ─────────────────────────────────────────────────────────────
const Menu::MenuItem Menu::_items[Menu::ITEM_COUNT] = {
    { "Calculator",   APP_CALCULATOR  },
    { "Text Editor",  APP_TEXT_EDITOR },
    { "Snake",        APP_SNAKE       },
    { "Settings",     APP_SETTINGS    },
    { "Home",         APP_LAUNCHER    },
};

// ─────────────────────────────────────────────────────────────
void Menu::begin() {
    Serial.println(F("[APP] Menu opened"));
    _selected = 0;
    _draw();
}

// ─────────────────────────────────────────────────────────────
void Menu::update() {
    // Nothing to tick — event-driven only
}

// ─────────────────────────────────────────────────────────────
void Menu::onInput(InputEvent ev) {
    switch (ev) {
        case INPUT_UP:
            if (_selected > 0) {
                _drawItem(_selected, false);
                _selected--;
                _drawItem(_selected, true);
            }
            break;

        case INPUT_DOWN:
            if (_selected < ITEM_COUNT - 1) {
                _drawItem(_selected, false);
                _selected++;
                _drawItem(_selected, true);
            }
            break;

        case INPUT_OK:
            Serial.print(F("[APP] Launching: "));
            Serial.println(_items[_selected].label);
            AppMgr.launchApp(_items[_selected].appID);
            break;

        case INPUT_BACK:
            AppMgr.launchApp(APP_LAUNCHER);
            break;

        default:
            break;
    }
}

// ─────────────────────────────────────────────────────────────
void Menu::_draw() {
    Display.clear(C_BLACK);
    Display.drawHeader("MENU", C_NOKIA_BLUE, C_WHITE);
    Display.drawDivider(16, C_DARK_GREY);

    for (uint8_t i = 0; i < ITEM_COUNT; i++) {
        _drawItem(i, i == _selected);
    }

    // Soft-key bar
    Display.fillRect(0, 148, SCREEN_W, 12, C_DARK_GREY);
    Display.setTextColour(C_WHITE, C_DARK_GREY);
    Display.setTextSize(1);
    Display.setCursor(4, 150);
    Display.print("OK=Open");
    Display.setCursor(SCREEN_W - 34, 150);
    Display.print("B=Back");
}

// ─────────────────────────────────────────────────────────────
// Each item occupies a 22px tall row starting at y=18
// ─────────────────────────────────────────────────────────────
void Menu::_drawItem(uint8_t index, bool highlighted) {
    int16_t y  = 18 + index * 24;
    uint16_t bg = highlighted ? C_NOKIA_BLUE : C_BLACK;
    uint16_t fg = C_WHITE;

    Display.fillRect(0, y, SCREEN_W, 22, bg);

    // Arrow indicator
    Display.setTextColour(fg, bg);
    Display.setTextSize(1);
    Display.setCursor(4, y + 7);
    Display.print(highlighted ? ">" : " ");

    // Label
    Display.setCursor(14, y + 7);
    Display.print(_items[index].label);
}
