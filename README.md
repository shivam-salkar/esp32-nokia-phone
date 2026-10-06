# ESP32-Nokia-OS

A Nokia-style mini operating system for **ESP32-S3** with a **1.8" ST7735 TFT (128×160)**.

---

## Hardware

| Component | Details |
|-----------|---------|
| MCU | ESP32-S3 Dev Module |
| Display | 1.8" TFT SPI 128×160 V1.1 (ST7735 / ST7735S) |
| Input (current) | Serial Monitor |
| Input (future) | Physical GPIO buttons |

### Wiring

| TFT Pin | ESP32-S3 GPIO |
|---------|--------------|
| VCC | 3V3 |
| GND | GND |
| CS | GPIO **10** |
| RESET | GPIO **8** |
| A0 / DC | GPIO **3** |
| SDA / MOSI | GPIO **11** |
| SCK | GPIO **12** |
| LED | 3V3 |

---

## Required Libraries (Arduino Library Manager)

- **Adafruit GFX Library**
- **Adafruit ST7735 and ST7789 Library**

---

## Arduino IDE Settings

| Setting | Value |
|---------|-------|
| Board | ESP32S3 Dev Module |
| Flash Mode | QIO |
| Partition Scheme | Default 4MB with spiffs |
| Upload Speed | 921600 |
| Serial Monitor Baud | 115200 |
| Serial Monitor Line Ending | Newline |

---

## Getting Started

### Step 1 — Verify TFT (Phase 0)

Open and upload **`TFT_Test/TFT_Test.ino`** first.

Expected output on Serial Monitor:
```
=== TFT TEST ===
Initialising TFT...
TFT initialised
RED
GREEN
BLUE
TEXT TEST
SHAPES
DONE
=== TFT TEST COMPLETE ===
```

The screen should cycle RED → GREEN → BLUE → BLACK with text and shapes.

> ⚠️ If the screen stays white or blank, stop here and debug the wiring / `initR()` variant before proceeding.

### Step 2 — Run the OS

Open and upload **`ESP32-Nokia-OS.ino`**.

Expected boot output:
```
======================
  ESP32-Nokia-OS Boot
======================
[BOOT] Starting OS
[DISPLAY] Initialising TFT...
[DISPLAY] TFT initialised OK
[DISPLAY] Screen: 128x160
[INPUT] Serial input ready
[INPUT] Commands: UP DOWN LEFT RIGHT OK BACK MENU
[INPUT] Aliases : u  d    l    r     o  b    m
[APP] AppManager ready
[APP] Launching app ID: 0
[APP] Launcher started
[BOOT] OS ready
```

---

## Serial Monitor Controls

Type commands in the Serial Monitor (with Newline line ending):

| Command | Alias | Action |
|---------|-------|--------|
| `UP` | `u` | Move up |
| `DOWN` | `d` | Move down |
| `LEFT` | `l` | Move left |
| `RIGHT` | `r` | Move right |
| `OK` | `o` | Confirm / select |
| `BACK` | `b` | Back / cancel |
| `MENU` | `m` | Open menu |

---

## Application Navigation

```
Home Screen
  └── OK / MENU → Main Menu
        ├── Calculator   (OK to open)
        ├── Text Editor  (OK to open)
        ├── Snake        (OK to open)
        ├── Settings     (OK to open)
        └── Home         (OK to return to launcher)
```

### Calculator
- UP/DOWN/LEFT/RIGHT → move cursor on keypad
- OK → press highlighted key
- BACK → return to menu

### Text Editor
- MENU → new note (type in Serial, then type `SAVE` or `CANCEL`)
- UP/DOWN → scroll file list
- OK → view selected note
- LEFT (in viewer) → delete note
- BACK → return to menu

### Snake
- UP/DOWN/LEFT/RIGHT → steer
- OK → restart after game over
- BACK → return to menu

### Settings
- UP/DOWN → navigate items
- OK → enter / toggle
- LEFT/RIGHT (in Theme) → cycle themes
- BACK → return to menu

---

## Project Structure

```
ESP32-Nokia-OS/
├── ESP32-Nokia-OS.ino        ← Main sketch
├── TFT_Test/
│   └── TFT_Test.ino          ← Phase 0 hardware test
├── config/
│   └── pins.h                ← GPIO pin assignments
├── core/
│   ├── OS.h / OS.cpp
│   ├── InputManager.h / .cpp
│   ├── DisplayManager.h / .cpp
│   └── AppManager.h / .cpp
├── ui/
│   ├── Launcher.h / .cpp     ← Home screen
│   └── Menu.h / .cpp         ← Main menu
└── apps/
    ├── Calculator/
    ├── TextEditor/
    ├── Settings/
    └── Snake/
```

---

## Architecture

```
                 ┌───────────────────┐
                 │    NokiaOS        │
                 └─────────┬─────────┘
                           │
       ┌───────────────────┼───────────────────┐
       │                   │                   │
       ▼                   ▼                   ▼
 InputManager        DisplayManager        AppManager
       │                   │                   │
  Serial Monitor          TFT          ┌───────┼───────┐
  (Physical buttons      128×160       │       │       │
   future scope)                  Calculator  Snake  ...
```

**Core rule:** Apps receive `InputEvent` values only — never raw Serial reads or GPIO states.

---

## Future Scope

- Physical GPIO buttons (same `InputEvent` enum, no app rewrites needed)
- Wi-Fi scanner / settings
- Bluetooth utilities
- More games
- RTC for real clock
- MicroSD storage
- Buzzer / speaker
- Battery + charging circuit
