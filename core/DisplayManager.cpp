// =============================================================
// DisplayManager.cpp — TFT Abstraction Layer Implementation
// =============================================================

#include "DisplayManager.h"

// Singleton instance
// Constructor: Adafruit_ST7735(CS, DC, MOSI, SCK, RST)
DisplayManager Display;

// ─────────────────────────────────────────────────────────────
// Private member init — use hardware SPI constructor
// We initialise _tft in begin() using custom SPI pins for ESP32-S3
// because the default SPI bus may not match our wiring.
// ─────────────────────────────────────────────────────────────

// We use the software-SPI style constructor so we can specify
// MOSI and SCK explicitly (ESP32-S3 flexible GPIO routing).
// Adafruit_ST7735(CS, DC, MOSI, SCK, RST)
static Adafruit_ST7735 _tftInstance(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCK, TFT_RST);

// ─────────────────────────────────────────────────────────────
void DisplayManager::begin() {
    Serial.println(F("[DISPLAY] Initialising TFT..."));

    _tftInstance.initR(INITR_BLACKTAB);   // ST7735S black-tab init
    _tftInstance.setRotation(0);          // Portrait 128×160
    _tftInstance.fillScreen(ST77XX_BLACK);

    Serial.println(F("[DISPLAY] TFT initialised OK"));
    Serial.print(F("[DISPLAY] Screen: "));
    Serial.print(SCREEN_W);
    Serial.print('x');
    Serial.println(SCREEN_H);
}

// ─────────────────────────────────────────────────────────────
void DisplayManager::clear(uint16_t colour) {
    _tftInstance.fillScreen(colour);
}

// ─────────────────────────────────────────────────────────────
void DisplayManager::setTextColour(uint16_t fg, uint16_t bg) {
    _tftInstance.setTextColor(fg, bg);
}

void DisplayManager::setTextSize(uint8_t size) {
    _tftInstance.setTextSize(size);
}

void DisplayManager::setCursor(int16_t x, int16_t y) {
    _tftInstance.setCursor(x, y);
}

void DisplayManager::print(const char* text) {
    _tftInstance.print(text);
}

void DisplayManager::println(const char* text) {
    _tftInstance.println(text);
}

// ─────────────────────────────────────────────────────────────
void DisplayManager::printCentered(const char* text, int16_t y,
                                    uint8_t size, uint16_t fg, uint16_t bg) {
    _tftInstance.setTextSize(size);
    _tftInstance.setTextColor(fg, bg);

    // Each character is 6px wide at size 1 (5+1 spacing)
    int16_t charW = 6 * size;
    int16_t len   = (int16_t)strlen(text);
    int16_t x     = (SCREEN_W - len * charW) / 2;
    if (x < 0) x = 0;

    _tftInstance.setCursor(x, y);
    _tftInstance.print(text);
}

// ─────────────────────────────────────────────────────────────
void DisplayManager::fillRect(int16_t x, int16_t y, int16_t w, int16_t h,
                               uint16_t colour) {
    _tftInstance.fillRect(x, y, w, h, colour);
}

void DisplayManager::drawRect(int16_t x, int16_t y, int16_t w, int16_t h,
                               uint16_t colour) {
    _tftInstance.drawRect(x, y, w, h, colour);
}

void DisplayManager::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                               uint16_t colour) {
    _tftInstance.drawLine(x0, y0, x1, y1, colour);
}

void DisplayManager::fillScreen(uint16_t colour) {
    _tftInstance.fillScreen(colour);
}

void DisplayManager::drawPixel(int16_t x, int16_t y, uint16_t colour) {
    _tftInstance.drawPixel(x, y, colour);
}

void DisplayManager::fillCircle(int16_t x, int16_t y, int16_t r,
                                 uint16_t colour) {
    _tftInstance.fillCircle(x, y, r, colour);
}

// ─────────────────────────────────────────────────────────────
// Header bar — 16px tall, full width
// ─────────────────────────────────────────────────────────────
void DisplayManager::drawHeader(const char* title, uint16_t bg, uint16_t fg) {
    _tftInstance.fillRect(0, 0, SCREEN_W, 16, bg);
    _tftInstance.setTextColor(fg, bg);
    _tftInstance.setTextSize(1);

    int16_t len = (int16_t)strlen(title);
    int16_t x   = (SCREEN_W - len * 6) / 2;
    if (x < 0) x = 0;
    _tftInstance.setCursor(x, 4);
    _tftInstance.print(title);
}

// ─────────────────────────────────────────────────────────────
// Status bar — 10px tall at bottom
// ─────────────────────────────────────────────────────────────
void DisplayManager::drawStatusBar(const char* left, const char* right,
                                    uint16_t bg, uint16_t fg) {
    int16_t y = SCREEN_H - 10;
    _tftInstance.fillRect(0, y, SCREEN_W, 10, bg);
    _tftInstance.setTextColor(fg, bg);
    _tftInstance.setTextSize(1);

    _tftInstance.setCursor(2, y + 1);
    _tftInstance.print(left);

    int16_t rLen = (int16_t)strlen(right);
    _tftInstance.setCursor(SCREEN_W - rLen * 6 - 2, y + 1);
    _tftInstance.print(right);
}

// ─────────────────────────────────────────────────────────────
void DisplayManager::drawDivider(int16_t y, uint16_t colour) {
    _tftInstance.drawLine(0, y, SCREEN_W - 1, y, colour);
}

// ─────────────────────────────────────────────────────────────
Adafruit_ST7735& DisplayManager::tft() {
    return _tftInstance;
}
