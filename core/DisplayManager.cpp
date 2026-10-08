// =============================================================
// DisplayManager.cpp — TFT Abstraction Layer Implementation
// =============================================================

#include "DisplayManager.h"

// Singleton instance
DisplayManager Display;

// Exact verified constructor from TFT_Test.ino:
// Explicit GPIO routing for ESP32-S3 (CS, DC, MOSI, SCK, RST)
static Adafruit_ST7735 _tftInstance(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCK, TFT_RST);

// ─────────────────────────────────────────────────────────────
void DisplayManager::begin() {
    Serial.println(F("[DISPLAY] Initializing TFT..."));

    // Hardware reset pulse to guarantee clean ST7735 controller start
    pinMode(TFT_RST, OUTPUT);
    digitalWrite(TFT_RST, HIGH);
    delay(10);
    digitalWrite(TFT_RST, LOW);
    delay(20);
    digitalWrite(TFT_RST, HIGH);
    delay(50);

    // Initialise ST7735S (Black Tab 128×160)
    _tftInstance.initR(INITR_BLACKTAB);
    _tftInstance.setRotation(0);          // Portrait 128×160
    _tftInstance.fillScreen(ST77XX_BLACK);

    Serial.println(F("[DISPLAY] TFT OK"));
}

// ─────────────────────────────────────────────────────────────
void DisplayManager::showBootScreen() {
    clear(C_BLACK);

    // Outer decorative borders
    drawRect(3, 3, SCREEN_W - 6, SCREEN_H - 6, C_NOKIA_BLUE);
    drawRect(5, 5, SCREEN_W - 10, SCREEN_H - 10, C_DARK_GREY);

    // Header box
    fillRect(10, 20, SCREEN_W - 20, 24, C_NOKIA_BLUE);
    printCentered("ESP32 MINI OS", 28, 1, C_WHITE, C_NOKIA_BLUE);

    // Retro mini phone graphic in center
    fillRoundRect(SCREEN_W / 2 - 14, 56, 28, 36, 4, C_DARK_GREY);
    drawRoundRect(SCREEN_W / 2 - 14, 56, 28, 36, 4, C_WHITE);
    fillRect(SCREEN_W / 2 - 9, 61, 18, 14, C_NOKIA_CYAN);
    // Keypad grid dots on graphic
    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 3; c++) {
            drawPixel(SCREEN_W / 2 - 6 + c * 5, 80 + r * 3, C_WHITE);
        }
    }

    // Subtitle
    printCentered("Starting...", 106, 1, C_LIGHT_GREY, C_BLACK);

    // Animated boot progress bar
    int barX = 24;
    int barY = 124;
    int barW = SCREEN_W - 48;
    int barH = 7;
    drawRect(barX, barY, barW, barH, C_DARK_GREY);
    for (int w = 0; w <= barW - 4; w += 6) {
        fillRect(barX + 2, barY + 2, w, barH - 4, C_NOKIA_CYAN);
        delay(40);
    }
    delay(200);
}

// ─────────────────────────────────────────────────────────────
void DisplayManager::clear(uint16_t colour) {
    _tftInstance.fillScreen(colour);
}

void DisplayManager::fillScreen(uint16_t colour) {
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

    int16_t charW = 6 * size;
    int16_t len   = (int16_t)strlen(text);
    int16_t x     = (SCREEN_W - len * charW) / 2;
    if (x < 0) x = 0;

    _tftInstance.setCursor(x, y);
    _tftInstance.print(text);
}

// ─────────────────────────────────────────────────────────────
void DisplayManager::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t colour) {
    _tftInstance.fillRect(x, y, w, h, colour);
}

void DisplayManager::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t colour) {
    _tftInstance.drawRect(x, y, w, h, colour);
}

void DisplayManager::drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t colour) {
    _tftInstance.drawRoundRect(x, y, w, h, r, colour);
}

void DisplayManager::fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t colour) {
    _tftInstance.fillRoundRect(x, y, w, h, r, colour);
}

void DisplayManager::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t colour) {
    _tftInstance.drawLine(x0, y0, x1, y1, colour);
}

void DisplayManager::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t colour) {
    _tftInstance.drawFastHLine(x, y, w, colour);
}

void DisplayManager::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t colour) {
    _tftInstance.drawFastVLine(x, y, h, colour);
}

void DisplayManager::drawPixel(int16_t x, int16_t y, uint16_t colour) {
    _tftInstance.drawPixel(x, y, colour);
}

void DisplayManager::fillCircle(int16_t x, int16_t y, int16_t r, uint16_t colour) {
    _tftInstance.fillCircle(x, y, r, colour);
}

void DisplayManager::drawCircle(int16_t x, int16_t y, int16_t r, uint16_t colour) {
    _tftInstance.drawCircle(x, y, r, colour);
}

// ─────────────────────────────────────────────────────────────
void DisplayManager::drawHeader(const char* title, uint16_t bg, uint16_t fg) {
    _tftInstance.fillRect(0, 0, SCREEN_W, 14, bg);
    _tftInstance.setTextColor(fg, bg);
    _tftInstance.setTextSize(1);

    int16_t len = (int16_t)strlen(title);
    int16_t x   = (SCREEN_W - len * 6) / 2;
    if (x < 0) x = 0;
    _tftInstance.setCursor(x, 3);
    _tftInstance.print(title);
    _tftInstance.drawFastHLine(0, 14, SCREEN_W, C_DARK_GREY);
}

// ─────────────────────────────────────────────────────────────
void DisplayManager::drawStatusBar(const char* left, const char* right, uint16_t bg, uint16_t fg) {
    _tftInstance.fillRect(0, 0, SCREEN_W, 12, bg);
    _tftInstance.setTextColor(fg, bg);
    _tftInstance.setTextSize(1);

    _tftInstance.setCursor(2, 2);
    _tftInstance.print(left);

    int16_t rLen = (int16_t)strlen(right);
    _tftInstance.setCursor(SCREEN_W - rLen * 6 - 2, 2);
    _tftInstance.print(right);
    _tftInstance.drawFastHLine(0, 12, SCREEN_W, C_DARK_GREY);
}

// ─────────────────────────────────────────────────────────────
void DisplayManager::drawSoftKeys(const char* left, const char* right, uint16_t bg, uint16_t fg) {
    int16_t y = SCREEN_H - 12;
    _tftInstance.drawFastHLine(0, y - 1, SCREEN_W, C_DARK_GREY);
    _tftInstance.fillRect(0, y, SCREEN_W, 12, bg);
    _tftInstance.setTextColor(fg, bg);
    _tftInstance.setTextSize(1);

    if (left && strlen(left) > 0) {
        _tftInstance.setCursor(3, y + 2);
        _tftInstance.print(left);
    }

    if (right && strlen(right) > 0) {
        int16_t rLen = (int16_t)strlen(right);
        _tftInstance.setCursor(SCREEN_W - rLen * 6 - 3, y + 2);
        _tftInstance.print(right);
    }
}

// ─────────────────────────────────────────────────────────────
void DisplayManager::drawDivider(int16_t y, uint16_t colour) {
    _tftInstance.drawFastHLine(0, y, SCREEN_W, colour);
}

// ─────────────────────────────────────────────────────────────
void DisplayManager::drawWifiIndicator(int16_t x, int16_t y, bool connected) {
    uint16_t col = connected ? C_GREEN : C_MID_GREY;
    _tftInstance.drawFastHLine(x + 3, y + 6, 2, col);
    _tftInstance.drawFastHLine(x + 2, y + 4, 4, col);
    _tftInstance.drawFastHLine(x + 1, y + 2, 6, col);
    _tftInstance.drawFastHLine(x,     y,     8, col);
}

// ─────────────────────────────────────────────────────────────
Adafruit_ST7735& DisplayManager::tft() {
    return _tftInstance;
}
