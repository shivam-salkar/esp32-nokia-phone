// =============================================================
// Menu.h — Main Application Menu
// =============================================================

#ifndef MENU_H
#define MENU_H

#include "../core/AppManager.h"
#include "../core/InputManager.h"

class Menu : public App {
public:
    void begin()  override;
    void update() override;
    void onInput(InputEvent ev) override;

private:
    static const uint8_t ITEM_COUNT = 5;

    struct MenuItem {
        const char* label;
        AppID       appID;
    };

    static const MenuItem _items[ITEM_COUNT];

    uint8_t _selected;

    void _draw();
    void _drawItem(uint8_t index, bool highlighted);
};

#endif // MENU_H
