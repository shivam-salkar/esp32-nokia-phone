// =============================================================
// TFT_Test.ino — Phase 0: Hardware Verification
// =============================================================
// Upload this STANDALONE sketch first.
// Confirm each colour and text appears on screen.
// Only move on to ESP32-Nokia-OS.ino when this passes.
//
// Required libraries (install via Arduino Library Manager):
//   - Adafruit GFX Library
//   - Adafruit ST7735 and ST7789 Library
//
// Wiring:
//   TFT VCC   → ESP32 3V3
//   TFT GND   → ESP32 GND
//   TFT CS    → GPIO 10
//   TFT RST   → GPIO 8
//   TFT DC    → GPIO 3
//   TFT MOSI  → GPIO 11
//   TFT SCK   → GPIO 12
//   TFT LED   → ESP32 3V3
// =============================================================

#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>

// ── Pin definitions ──────────────────────────────────────────
#define TFT_CS    10
#define TFT_RST   8
#define TFT_DC    3
#define TFT_MOSI  11
#define TFT_SCK   12

// Software-SPI constructor for explicit GPIO routing on ESP32-S3
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCK, TFT_RST);

// ─────────────────────────────────────────────────────────────
void setup() {
    Serial.begin(115200);
    delay(500);

    Serial.println(F("=== TFT TEST ==="));

    // ── TFT init ─────────────────────────────────────────────
    Serial.println(F("Initialising TFT..."));
    tft.initR(INITR_BLACKTAB);
    tft.setRotation(0);
    Serial.println(F("TFT initialised"));

    // ── RED ──────────────────────────────────────────────────
    Serial.println(F("RED"));
    tft.fillScreen(ST77XX_RED);
    delay(1000);

    // ── GREEN ─────────────────────────────────────────────────
    Serial.println(F("GREEN"));
    tft.fillScreen(ST77XX_GREEN);
    delay(1000);

    // ── BLUE ──────────────────────────────────────────────────
    Serial.println(F("BLUE"));
    tft.fillScreen(ST77XX_BLUE);
    delay(1000);

    // ── BLACK + TEXT ──────────────────────────────────────────
    Serial.println(F("TEXT TEST"));
    tft.fillScreen(ST77XX_BLACK);
    tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
    tft.setTextSize(2);
    tft.setCursor(10, 20);
    tft.println("ESP32-S3");
    tft.setTextSize(1);
    tft.setCursor(10, 50);
    tft.println("Nokia-style OS");
    tft.setCursor(10, 65);
    tft.println("TFT: ST7735");
    tft.setCursor(10, 80);
    tft.println("128 x 160 px");

    // ── Shapes ───────────────────────────────────────────────
    Serial.println(F("SHAPES"));
    tft.drawRect(5,  100, 118, 50, ST77XX_WHITE);
    tft.fillRect(10, 105,  30, 40, ST77XX_RED);
    tft.fillRect(48, 105,  30, 40, ST77XX_GREEN);
    tft.fillRect(84, 105,  30, 40, ST77XX_BLUE);
    tft.drawLine(5,  96, 123, 96,  ST77XX_YELLOW);

    // ── Done ─────────────────────────────────────────────────
    tft.setTextColor(ST77XX_YELLOW, ST77XX_BLACK);
    tft.setTextSize(1);
    tft.setCursor(20, 158);
    tft.print("  ALL DONE  ");
    Serial.println(F("DONE"));
    Serial.println(F("=== TFT TEST COMPLETE ==="));
}

void loop() {
    // Nothing — test is one-shot
}
