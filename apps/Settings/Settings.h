// =============================================================
// Settings.h — System Settings App
// =============================================================

#ifndef SETTINGS_H
#define SETTINGS_H

#include "../../core/AppManager.h"
#include "../../core/InputManager.h"
#include <Preferences.h>

class Settings : public App {
public:
    void begin()  override;
    void update() override;
    void onInput(InputEvent ev) override;

private:
    enum SettingsMode { MODE_LIST, MODE_THEME, MODE_ABOUT };

    SettingsMode _mode;

    struct SettingItem {
        const char* label;
        const char* value;
    };

    static const uint8_t ITEM_COUNT = 4;
    uint8_t _selected;

    // Persistent settings
    uint8_t _theme;       // 0=Dark, 1=Light
    uint8_t _brightness;  // 0–4 (visual scale; no PWM pin yet)

    Preferences _prefs;

    void _load();
    void _save();

    void _drawList();
    void _drawTheme();
    void _drawAbout();
    void _drawItem(uint8_t idx, bool hl);
    const char* _themeLabel();

    static const char* _labels[ITEM_COUNT];
};

#endif // SETTINGS_H
