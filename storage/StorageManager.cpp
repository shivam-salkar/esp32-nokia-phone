// =============================================================
// StorageManager.cpp — LittleFS & Preferences Storage Manager
// =============================================================

#include "StorageManager.h"

StorageManager Storage;

static const char* PREFS_NS = "nokia_os";

void StorageManager::begin() {
    Serial.println(F("[STORAGE] Initializing LittleFS..."));
    _fsReady = LittleFS.begin(true); // true = format on fail
    if (_fsReady) {
        Serial.println(F("[STORAGE] OK"));
        if (!LittleFS.exists("/notes")) {
            LittleFS.mkdir("/notes");
        }
    } else {
        Serial.println(F("[STORAGE] ERROR: LittleFS mount failed"));
    }
}

size_t StorageManager::totalBytes() {
    if (!_fsReady) return 0;
    return LittleFS.totalBytes();
}

size_t StorageManager::usedBytes() {
    if (!_fsReady) return 0;
    return LittleFS.usedBytes();
}

uint8_t StorageManager::getTheme() {
    Preferences prefs;
    prefs.begin(PREFS_NS, true);
    uint8_t val = prefs.getUChar("theme", 0); // 0=Retro Blue, 1=Dark, 2=Light
    prefs.end();
    return val;
}

void StorageManager::setTheme(uint8_t theme) {
    Preferences prefs;
    prefs.begin(PREFS_NS, false);
    prefs.putUChar("theme", theme);
    prefs.end();
}

uint8_t StorageManager::getBrightness() {
    Preferences prefs;
    prefs.begin(PREFS_NS, true);
    uint8_t val = prefs.getUChar("bright", 3);
    prefs.end();
    return val;
}

void StorageManager::setBrightness(uint8_t brightness) {
    Preferences prefs;
    prefs.begin(PREFS_NS, false);
    prefs.putUChar("bright", brightness);
    prefs.end();
}

uint16_t StorageManager::getSnakeHighScore() {
    Preferences prefs;
    prefs.begin(PREFS_NS, true);
    uint16_t val = prefs.getUShort("snake_hi", 0);
    prefs.end();
    return val;
}

void StorageManager::setSnakeHighScore(uint16_t score) {
    Preferences prefs;
    prefs.begin(PREFS_NS, false);
    prefs.putUShort("snake_hi", score);
    prefs.end();
}
