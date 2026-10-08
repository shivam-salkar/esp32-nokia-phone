// =============================================================
// DisplayManager.h — TFT Abstraction Layer
// =============================================================
// ST7735 1.8" 128×160 TFT Controller
// =============================================================

#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>
#include "../config/pins.h"

// ─────────────────────────────────────────────────────────────
// Screen dimensions
// ─────────────────────────────────────────────────────────────
#define SCREEN_W  128
#define SCREEN_H  160

// ─────────────────────────────────────────────────────────────
// Colour palette (RGB565)
// ─────────────────────────────────────────────────────────────
#define C_BLACK       0x0000
#define C_WHITE       0xFFFF
#define C_RED         0xF800
#define C_GREEN       0x07E0
#define C_BLUE        0x001F
#define C_CYAN        0x07FF
#define C_MAGENTA     0xF81F
#define C_YELLOW      0xFFE0
#define C_ORANGE      0xFD20
#define C_DARK_GREY   0x2104
#define C_MID_GREY    0x632C
#define C_LIGHT_GREY  0xC618
#define C_NOKIA_BLUE  0x0275   // Classic Nokia blue
#define C_NOKIA_CYAN  0x05BF   // Highlight cyan
#define C_NOKIA_GREEN 0x0660   // Nokia accent green

class DisplayManager {
public:
    void begin();
    void showBootScreen();

    // ── Clear & Base ─────────────────────────────────────────
    void clear(uint16_t colour = C_BLACK);
    void fillScreen(uint16_t colour);

    // ── Text ─────────────────────────────────────────────────
    void setTextColour(uint16_t fg, uint16_t bg = C_BLACK);
    void setTextSize(uint8_t size);
    void setCursor(int16_t x, int16_t y);
    void print(const char* text);
    void println(const char* text);
    void printCentered(const char* text, int16_t y, uint8_t size = 1,
                       uint16_t fg = C_WHITE, uint16_t bg = C_BLACK);

    // ── Shapes ───────────────────────────────────────────────
    void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t colour);
    void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t colour);
    void drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t colour);
    void fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t colour);
    void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t colour);
    void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t colour);
    void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t colour);
    void drawPixel(int16_t x, int16_t y, uint16_t colour);
    void fillCircle(int16_t x, int16_t y, int16_t r, uint16_t colour);
    void drawCircle(int16_t x, int16_t y, int16_t r, uint16_t colour);

    // ── UI Components ────────────────────────────────────────
    void drawHeader(const char* title, uint16_t bg = C_NOKIA_BLUE, uint16_t fg = C_WHITE);
    void drawStatusBar(const char* left, const char* right, uint16_t bg = C_DARK_GREY, uint16_t fg = C_WHITE);
    void drawSoftKeys(const char* left, const char* right, uint16_t bg = C_NOKIA_BLUE, uint16_t fg = C_WHITE);
    void drawDivider(int16_t y, uint16_t colour = C_DARK_GREY);
    void drawWifiIndicator(int16_t x, int16_t y, bool connected);

    // Direct access
    Adafruit_ST7735& tft();
};

extern DisplayManager Display;

#endif // DISPLAY_MANAGER_H
