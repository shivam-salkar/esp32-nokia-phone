// =============================================================
// StorageManager.h — LittleFS & Preferences Storage Manager
// =============================================================

#ifndef STORAGE_MANAGER_H
#define STORAGE_MANAGER_H

#include <Arduino.h>
#include <LittleFS.h>
#include <Preferences.h>

class StorageManager {
public:
    void begin();
    bool isLittleFSReady() const { return _fsReady; }

    // LittleFS metrics
    size_t totalBytes();
    size_t usedBytes();

    // Preferences for OS settings
    uint8_t  getTheme();
    void     setTheme(uint8_t theme);

    uint8_t  getBrightness();
    void     setBrightness(uint8_t brightness);

    uint16_t getSnakeHighScore();
    void     setSnakeHighScore(uint16_t score);

private:
    bool _fsReady;
};

extern StorageManager Storage;

#endif // STORAGE_MANAGER_H
