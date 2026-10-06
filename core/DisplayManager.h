// =============================================================
// DisplayManager.h — TFT Abstraction Layer
// =============================================================
// Wraps Adafruit ST7735 so that all UI code uses one clean API.
// Screen resolution: 128 × 160 (ST7735 INITR_BLACKTAB)
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
#define C_DARK_GREY   0x4208
#define C_LIGHT_GREY  0xC618
#define C_NOKIA_BLUE  0x0319   // Deep Nokia-ish blue
#define C_NOKIA_GREEN 0x0600   // Status green

// ─────────────────────────────────────────────────────────────
// DisplayManager
// ─────────────────────────────────────────────────────────────
class DisplayManager {
public:
    void begin();

    // ── Clear ──────────────────────────────────────────────
    void clear(uint16_t colour = C_BLACK);

    // ── Text ───────────────────────────────────────────────
    void setTextColour(uint16_t fg, uint16_t bg = C_BLACK);
    void setTextSize(uint8_t size);
    void setCursor(int16_t x, int16_t y);
    void print(const char* text);
    void println(const char* text);
    void printCentered(const char* text, int16_t y, uint8_t size = 1,
                       uint16_t fg = C_WHITE, uint16_t bg = C_BLACK);

    // ── Shapes ─────────────────────────────────────────────
    void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t colour);
    void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t colour);
    void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t colour);
    void fillScreen(uint16_t colour);
    void drawPixel(int16_t x, int16_t y, uint16_t colour);
    void fillCircle(int16_t x, int16_t y, int16_t r, uint16_t colour);

    // ── Header bar ─────────────────────────────────────────
    void drawHeader(const char* title, uint16_t bg = C_NOKIA_BLUE,
                    uint16_t fg = C_WHITE);

    // ── Status bar ─────────────────────────────────────────
    void drawStatusBar(const char* left, const char* right,
                       uint16_t bg = C_DARK_GREY, uint16_t fg = C_WHITE);

    // ── Divider line ───────────────────────────────────────
    void drawDivider(int16_t y, uint16_t colour = C_DARK_GREY);

    // ── Direct TFT access (for advanced use) ───────────────
    Adafruit_ST7735& tft();

    // Note: the Adafruit_ST7735 instance lives as a file-scope
    // static in DisplayManager.cpp — Adafruit_ST7735 has no
    // default constructor so it cannot be a class member.
};

// Singleton
extern DisplayManager Display;

#endif // DISPLAY_MANAGER_H
