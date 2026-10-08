// =============================================================
// pins.h — Hardware Pin Definitions
// =============================================================
// ESP32-S3 MINI OS — CONFIRMED HARDWARE PINS
// HARDWARE CONFIGURATION IS FINAL AND PHYSICALLY VERIFIED.
// DO NOT CHANGE ANY GPIO ASSIGNMENTS.
// =============================================================

#ifndef PINS_H
#define PINS_H

// ─────────────────────────────────────────────────────────────
// TFT 1.8" 128×160 SPI (ST7735 / ST7735S)
// ─────────────────────────────────────────────────────────────
// TFT VCC  → 3V3
// TFT GND  → GND
// TFT LED  → 3V3
// ─────────────────────────────────────────────────────────────
#define TFT_CS    10   // Chip Select (GPIO 10)
#define TFT_RST   8    // Reset (GPIO 8)
#define TFT_DC    3    // Data / Command (A0) (GPIO 3)
#define TFT_MOSI  11   // SDA / MOSI (GPIO 11)
#define TFT_SCK   12   // Clock (GPIO 12)
#define TFT_SCLK  12   // Alias for TFT Clock (GPIO 12)

// ─────────────────────────────────────────────────────────────
// 4×4 Matrix Keypad (Confirmed Working)
// ─────────────────────────────────────────────────────────────
// Physical connector:
// R1 → GPIO18
// R2 → GPIO17
// R3 → GPIO16
// R4 → GPIO15
//
// C1 → GPIO7
// C2 → GPIO6
// C3 → GPIO5
// C4 → GPIO4
// ─────────────────────────────────────────────────────────────
#define KEYPAD_ROWS   4
#define KEYPAD_COLS   4

#define KEYPAD_R1     18
#define KEYPAD_R2     17
#define KEYPAD_R3     16
#define KEYPAD_R4     15

#define KEYPAD_C1     7
#define KEYPAD_C2     6
#define KEYPAD_C3     5
#define KEYPAD_C4     4

// Compatibility aliases
#define KEYPAD_ROW_0  KEYPAD_R1
#define KEYPAD_ROW_1  KEYPAD_R2
#define KEYPAD_ROW_2  KEYPAD_R3
#define KEYPAD_ROW_3  KEYPAD_R4

#define KEYPAD_COL_0  KEYPAD_C1
#define KEYPAD_COL_1  KEYPAD_C2
#define KEYPAD_COL_2  KEYPAD_C3
#define KEYPAD_COL_3  KEYPAD_C4

#endif // PINS_H
