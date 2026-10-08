// =============================================================
// Menu.cpp — Main Application Menu
// =============================================================

#include "Menu.h"
#include "../core/DisplayManager.h"
#include "../core/AppManager.h"

const Menu::MenuItem Menu::_items[Menu::ITEM_COUNT] = {
    { "Calculator",   APP_CALCULATOR },
    { "Notes",        APP_NOTES      },
    { "Snake",        APP_SNAKE      },
    { "Settings",     APP_SETTINGS   },
    { "About",        APP_ABOUT      }
};

void Menu::begin() {
    Serial.println(F("[APP] Menu opened"));
    _selected = 0;
    _draw();
}

void Menu::update() {
    // Event-driven
}

void Menu::onInput(InputEvent ev) {
    switch (ev) {
        case INPUT_UP:
        case INPUT_KEY_2:
            if (_selected > 0) {
                _drawItem(_selected, false);
                _selected--;
                _drawItem(_selected, true);
            }
            break;

        case INPUT_DOWN:
        case INPUT_KEY_8:
            if (_selected < ITEM_COUNT - 1) {
                _drawItem(_selected, false);
                _selected++;
                _drawItem(_selected, true);
            }
            break;

        case INPUT_OK:
        case INPUT_KEY_5:
            Serial.print(F("[APP] Opening: "));
            Serial.println(_items[_selected].label);
            AppMgr.launchApp(_items[_selected].appID);
            break;

        case INPUT_BACK:
        case INPUT_STAR:
        case INPUT_HOME:
        case INPUT_KEY_0:
        case INPUT_MENU:
        case INPUT_HASH:
            AppMgr.launchApp(APP_LAUNCHER);
            break;

        default:
            break;
    }
}

void Menu::_draw() {
    Display.clear(C_BLACK);
    Display.drawHeader("MENU", C_NOKIA_BLUE, C_WHITE);

    for (uint8_t i = 0; i < ITEM_COUNT; i++) {
        _drawItem(i, i == _selected);
    }

    // Navigation hint line
    Display.drawDivider(136, C_DARK_GREY);
    Display.setTextColour(C_LIGHT_GREY, C_BLACK);
    Display.setTextSize(1);
    Display.printCentered("2/8 Nav   5 Select", 138, 1, C_LIGHT_GREY, C_BLACK);

    // Soft-key bar
    Display.drawSoftKeys("*: Back", "0: Home", C_NOKIA_BLUE, C_WHITE);
}

void Menu::_drawItem(uint8_t index, bool highlighted) {
    int16_t y   = 17 + index * 23;
    uint16_t bg = highlighted ? C_NOKIA_BLUE : C_BLACK;
    uint16_t fg = highlighted ? C_WHITE : C_LIGHT_GREY;

    Display.fillRect(2, y, SCREEN_W - 4, 21, bg);
    if (highlighted) {
        Display.drawRect(2, y, SCREEN_W - 4, 21, C_NOKIA_CYAN);
    }

    // Selection pointer
    Display.setTextColour(highlighted ? C_YELLOW : C_BLACK, bg);
    Display.setTextSize(1);
    Display.setCursor(6, y + 6);
    Display.print(highlighted ? ">" : " ");

    // Icon representation
    Display.drawRect(16, y + 5, 10, 10, highlighted ? C_WHITE : C_MID_GREY);
    Display.drawPixel(21, y + 10, highlighted ? C_YELLOW : C_MID_GREY);

    // Text label
    Display.setTextColour(fg, bg);
    Display.setCursor(30, y + 6);
    Display.print(_items[index].label);
}
