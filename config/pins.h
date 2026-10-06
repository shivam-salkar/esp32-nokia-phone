// =============================================================
// pins.h — Hardware pin definitions
// =============================================================
// Edit this file to match your wiring.
// All other source files include this header; no other file
// should hard-code GPIO numbers.
// =============================================================

#ifndef PINS_H
#define PINS_H

// ─────────────────────────────────────────────────────────────
// TFT SPI (ST7735 / 1.8" 128×160 V1.1)
// ─────────────────────────────────────────────────────────────
// TFT VCC   → ESP32 3V3
// TFT GND   → ESP32 GND
// TFT LED   → ESP32 3V3
// ─────────────────────────────────────────────────────────────
#define TFT_CS    10   // Chip Select
#define TFT_RST   8    // Reset
#define TFT_DC    3    // Data/Command (A0)
#define TFT_MOSI  11   // SDA / MOSI
#define TFT_SCK   12   // Clock

// ─────────────────────────────────────────────────────────────
// Physical Buttons — FUTURE SCOPE
// ─────────────────────────────────────────────────────────────
// Uncomment and assign GPIOs when buttons are wired up.
// #define BTN_UP    4
// #define BTN_DOWN  5
// #define BTN_LEFT  6
// #define BTN_RIGHT 7
// #define BTN_OK    9
// #define BTN_BACK  13
// #define BTN_MENU  14

#endif // PINS_H
