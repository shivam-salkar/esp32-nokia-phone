# ESP32-Nokia-OS

A complete retro Nokia-style mini operating system built for the **ESP32-S3** with a **1.8" ST7735 TFT (128×160)** and a **4×4 Membrane Keypad**, programmed purely in **Arduino IDE / Arduino-ESP32**.

---

## Hardware Configuration & Wiring

### 1. ESP32-S3 Dev Board
- **Flash**: 16 MB
- **PSRAM**: 8 MB

### 2. 1.8" TFT SPI (128×160 ST7735)
| TFT Pin | ESP32-S3 GPIO | Description |
|---|---|---|
| **VCC** | 3V3 | Power |
| **GND** | GND | Ground |
| **CS** | GPIO **10** | Chip Select |
| **RESET** | GPIO **8** | Hardware Reset |
| **A0 / DC** | GPIO **3** | Data / Command |
| **SDA / MOSI**| GPIO **11** | SPI Data Out |
| **SCK** | GPIO **12** | SPI Clock |
| **LED** | 3V3 | Backlight |

### 3. 4×4 Membrane Keypad
| Keypad Pin | ESP32-S3 GPIO | Function |
|---|---|---|
| **Row 1** | GPIO **18** | Row 0 (Keys: 1, 2, 3, A) |
| **Row 2** | GPIO **17** | Row 1 (Keys: 4, 5, 6, B) |
| **Row 3** | GPIO **16** | Row 2 (Keys: 7, 8, 9, C) |
| **Row 4** | GPIO **15** | Row 3 (Keys: *, 0, #, D) |
| **Col 1** | GPIO **7** | Col 0 (Keys: 1, 4, 7, *) |
| **Col 2** | GPIO **6** | Col 1 (Keys: 2, 5, 8, 0) |
| **Col 3** | GPIO **5** | Col 2 (Keys: 3, 6, 9, #) |
| **Col 4** | GPIO **4** | Col 3 (Keys: A, B, C, D) |

---

## Required Libraries (Arduino Library Manager)

1. **Adafruit GFX Library** (by Adafruit)
2. **Adafruit ST7735 and ST7789 Library** (by Adafruit)
3. **Keypad** (by Mark Stanley, Alexander Brevig)

---

## Arduino IDE Board Settings

- **Board**: `ESP32S3 Dev Module`
- **Flash Mode**: `QIO 80MHz`
- **Flash Size**: `16MB (128Mb)` (or match your module)
- **PSRAM**: `OPI PSRAM` (or Enabled)
- **Partition Scheme**: `Default 4MB with spiffs` (or any scheme with SPIFFS/LittleFS)
- **Upload Speed**: `921600`
- **USB CDC On Boot**: `Enabled`
- **Serial Monitor**: `115200` baud, Line ending: `Newline (\n)`

---

## System Architecture

```text
ESP32-Mini-OS/
├── ESP32-Nokia-OS.ino      # Main sketch entry point
├── config/
│   └── pins.h             # Verified TFT and Keypad GPIO pinouts
├── core/
│   ├── OS.h / OS.cpp       # Boot sequence & top-level coordinator
│   ├── InputManager.h/.cpp # Keypad & Serial input abstraction (InputEvent)
│   ├── DisplayManager.h/.cpp # TFT graphics layer, UI widgets, boot screen
│   └── AppManager.h/.cpp   # App lifecycle (Launcher, Menu, Apps)
├── apps/
│   ├── Calculator/         # 4-function floating-point calculator
│   ├── TextEditor/         # Notes app with Nokia multi-tap & LittleFS
│   ├── Snake/              # Classic Snake game with persistent high score
│   ├── Settings/           # Display, Theme, Storage, System info
│   └── About/              # Dynamic live ESP32-S3 hardware metrics
├── ui/
│   ├── Launcher.h/.cpp     # Nokia-style Home Screen with status bar & clock
│   └── Menu.h/.cpp         # Scrollable application menu
└── storage/
    └── StorageManager.h/.cpp # LittleFS filesystem & Preferences (NVS)
```

---

## Nokia-Style Navigation & Key Mapping

### Global OS Navigation (Home, Menu, Settings, Snake)
- **`2`** → **UP**
- **`8`** → **DOWN**
- **`4`** → **LEFT**
- **`6`** → **RIGHT**
- **`5`** → **OK / SELECT**
- **`*`** → **BACK**
- **`#`** → **MENU**
- **`0`** → **HOME**

*(Serial Monitor commands `UP`, `DOWN`, `LEFT`, `RIGHT`, `OK`, `BACK`, `MENU`, `HOME` or shortcuts `u`, `d`, `l`, `r`, `o`, `b`, `m`, `h` are also fully supported as debug input).*

---

## Applications

### 1. Home Screen (Launcher)
- Live digital clock with blinking colon.
- Status bar with honest Wi-Fi hardware status (no fake battery percentage).
- Nokia retro graphic frame and center emblem.
- Press **`#`** or **`5`** to open the Main Menu.

### 2. Main Menu
- Scrollable list of 5 applications:
  1. **Calculator**
  2. **Notes**
  3. **Snake**
  4. **Settings**
  5. **About**
- Navigate with **`2` / `8`**, select with **`5`**, back to Home with **`*`** or **`0`**.

### 3. Calculator
- Floating-point arithmetic (+, -, ×, ÷, =).
- **Direct 4×4 Keypad Entry**:
  - `1` – `9`: Enter digits
  - `0`: Enter zero (double-tap within 500ms enters decimal point `.`)
  - `A`: `+`
  - `B`: `-`
  - `C`: `×`
  - `D`: `÷`
  - `#`: `=` (Calculate result)
  - `*`: Backspace / Clear (exits to menu when input is empty)

### 4. Notes (Text Editor)
- Fully persisted in **LittleFS** under `/notes/`.
- Features: Create new note, view notes, edit notes, delete notes.
- **Nokia-Style Multi-Tap Text Entry**:
  - `1` → `.,!?-1`
  - `2` → `ABC2` (or `abc2`)
  - `3` → `DEF3` (or `def3`)
  - `4` → `GHI4` (or `ghi4`)
  - `5` → `JKL5` (or `jkl5`)
  - `6` → `MNO6` (or `mno6`)
  - `7` → `PQRS7` (or `pqrs7`)
  - `8` → `TUV8` (or `tuv8`)
  - `9` → `WXYZ9` (or `wxyz9`)
  - `0` → ` ` (Space), then `0`
  - `*` → Backspace / cancel
  - `#` or `A` → Save note to LittleFS
  - `C` → Toggle Caps Lock (ABC / abc)
  - `D` → Insert newline (`\n`)
  - 800ms auto-commit timer between repeated keypresses.

### 5. Snake
- Classic retro Nokia Snake with rounded cells and red food.
- Controls: `2` (UP), `8` (DOWN), `4` (LEFT), `6` (RIGHT).
- Difficulty increases dynamically as score increases.
- Persistent High Score saved to **Preferences / NVS**.
- Dedicated retro Start Screen, Game Over screen with instant restart (`5`).

### 6. Settings
- **Display**: Brightness levels (1–5) and ST7735 screen info.
- **Theme**: Retro Blue, Dark Matrix, and Classic Light.
- **Storage**: Real-time LittleFS total, used, free KB metrics and storage bar.
- **System**: ESP32-S3 chip revision, 240MHz CPU frequency, Flash size, PSRAM size, and free heap.
- **About**: Firmware and OS specifications.
- Settings are persistently saved to **Preferences / NVS**.

### 7. About
- Displays live hardware metrics queried directly from the ESP32-S3:
  - Flash Chip Size (MB)
  - PSRAM Size (MB)
  - Dynamic Free Heap (KB)
  - Software framework: Arduino-ESP32
