// =============================================================
// Settings.h — System Settings App
// =============================================================

#ifndef SETTINGS_H
#define SETTINGS_H

#include "../../core/AppManager.h"
#include "../../core/InputManager.h"

class Settings : public App {
public:
    void begin()  override;
    void update() override;
    void onInput(InputEvent ev) override;

private:
    enum SettingsSubView {
        VIEW_MAIN,
        VIEW_DISPLAY,
        VIEW_THEME,
        VIEW_STORAGE,
        VIEW_SYSTEM,
        VIEW_ABOUT
    };

    static const uint8_t ITEM_COUNT = 5;
    static const char* _menuItems[ITEM_COUNT];

    SettingsSubView _view;
    uint8_t         _selected;

    uint8_t _theme;       // 0=Retro Blue, 1=Dark, 2=Light
    uint8_t _brightness;  // 1–5

    void _drawMain();
    void _drawMenuItem(uint8_t idx, bool hl);
    void _drawDisplayView();
    void _drawThemeView();
    void _drawStorageView();
    void _drawSystemView();
    void _drawAboutView();
};

#endif // SETTINGS_H
