// =============================================================
// Snake.cpp — Classic Snake Game
// =============================================================
// Controls:
//   UP / DOWN / LEFT / RIGHT → steer
//   OK                       → restart after game over
//   BACK                     → return to menu
//
// Grid: 20×21 cells, 6px each
// Speed increases by 10ms every 5 food eaten.
// =============================================================

#include "Snake.h"
#include "../../core/DisplayManager.h"
#include "../../core/AppManager.h"

// ─────────────────────────────────────────────────────────────
void Snake::begin() {
    Serial.println(F("[APP] Snake started"));
    Serial.println(F("[SNAKE] UP/DOWN/LEFT/RIGHT=steer OK=restart B=menu"));

    _highScore = 0;  // Could persist via Preferences in future
    _score     = 0;
    _speed     = 250;
    _state     = STATE_PLAYING;

    // Initial snake: 3 cells long, centre of grid, heading right
    _len = 3;
    _body[0] = { 12, 10 };  // Head
    _body[1] = { 11, 10 };
    _body[2] = { 10, 10 };
    _dir     = DIR_RIGHT;
    _nextDir = DIR_RIGHT;

    randomSeed(analogRead(0));
    _spawnFood();

    _lastMove = millis();

    Display.clear(C_BLACK);
    _drawGrid();
    _drawScore();
    // Draw initial snake
    for (uint16_t i = 0; i < _len; i++) {
        uint16_t c = (i == 0) ? C_NOKIA_GREEN : 0x07C0;  // Bright head, darker body
        _drawCell(_body[i].x, _body[i].y, c);
    }
    _drawFood();
}

// ─────────────────────────────────────────────────────────────
void Snake::update() {
    if (_state == STATE_DEAD) return;

    uint32_t now = millis();
    if (now - _lastMove < _speed) return;
    _lastMove = now;

    _dir = _nextDir;
    _step();
}

// ─────────────────────────────────────────────────────────────
void Snake::onInput(InputEvent ev) {
    if (_state == STATE_DEAD) {
        if (ev == INPUT_OK)   { begin(); return; }
        if (ev == INPUT_BACK) { AppMgr.launchApp(APP_MENU); return; }
        return;
    }

    if (ev == INPUT_BACK) {
        AppMgr.launchApp(APP_MENU);
        return;
    }

    // Buffer next direction — prevent 180° reversal
    switch (ev) {
        case INPUT_UP:    if (_dir != DIR_DOWN)  _nextDir = DIR_UP;    break;
        case INPUT_DOWN:  if (_dir != DIR_UP)    _nextDir = DIR_DOWN;  break;
        case INPUT_LEFT:  if (_dir != DIR_RIGHT) _nextDir = DIR_LEFT;  break;
        case INPUT_RIGHT: if (_dir != DIR_LEFT)  _nextDir = DIR_RIGHT; break;
        default: break;
    }
}

// ─────────────────────────────────────────────────────────────
// Move snake one step
// ─────────────────────────────────────────────────────────────
void Snake::_step() {
    // Compute new head position
    int8_t dx = 0, dy = 0;
    switch (_dir) {
        case DIR_UP:    dy = -1; break;
        case DIR_DOWN:  dy =  1; break;
        case DIR_LEFT:  dx = -1; break;
        case DIR_RIGHT: dx =  1; break;
    }

    int8_t nx = (int8_t)_body[0].x + dx;
    int8_t ny = (int8_t)_body[0].y + dy;

    // Wall collision
    if (nx < 0 || nx >= COLS || ny < 0 || ny >= ROWS) {
        _state = STATE_DEAD;
        _drawGameOver();
        return;
    }

    // Self collision — skip tail (it will move away)
    if (_collision((uint8_t)nx, (uint8_t)ny, 1)) {
        _state = STATE_DEAD;
        _drawGameOver();
        return;
    }

    // Check food
    bool ate = (nx == _food.x && ny == _food.y);

    // Erase tail cell unless ate
    if (!ate) {
        _drawCell(_body[_len - 1].x, _body[_len - 1].y, C_BLACK);
    }

    // Shift body
    if (!ate) {
        // Move body segments
        for (uint16_t i = _len - 1; i > 0; i--) {
            _body[i] = _body[i - 1];
        }
    } else {
        // Grow: add one more segment at tail (no pop)
        if (_len < MAX_LEN) {
            for (uint16_t i = _len; i > 0; i--) {
                _body[i] = _body[i - 1];
            }
            _len++;
        } else {
            // Already max — just shift
            for (uint16_t i = _len - 1; i > 0; i--) {
                _body[i] = _body[i - 1];
            }
        }
    }

    // Place new head
    _body[0] = { (uint8_t)nx, (uint8_t)ny };

    // Recolour old head to body colour
    if (_len > 1) {
        _drawCell(_body[1].x, _body[1].y, 0x07C0);
    }
    // Draw new head
    _drawCell(_body[0].x, _body[0].y, C_NOKIA_GREEN);

    if (ate) {
        _score++;
        if (_score > _highScore) _highScore = _score;

        // Increase speed every 5 food
        if (_speed > 80 && _score % 5 == 0) {
            _speed -= 20;
        }

        _spawnFood();
        _drawFood();
        _drawScore();

        Serial.print(F("[SNAKE] Score: "));
        Serial.println(_score);
    }
}

// ─────────────────────────────────────────────────────────────
bool Snake::_collision(uint8_t x, uint8_t y, uint16_t skipTail) {
    uint16_t limit = (skipTail && _len > 0) ? _len - 1 : _len;
    for (uint16_t i = 0; i < limit; i++) {
        if (_body[i].x == x && _body[i].y == y) return true;
    }
    return false;
}

// ─────────────────────────────────────────────────────────────
void Snake::_spawnFood() {
    uint8_t fx, fy;
    do {
        fx = (uint8_t)(random(COLS));
        fy = (uint8_t)(random(ROWS));
    } while (_collision(fx, fy, 0));
    _food = { fx, fy };
}

// ─────────────────────────────────────────────────────────────
void Snake::_drawGrid() {
    // Play area border
    Display.fillRect(0, 0, SCREEN_W, SCREEN_H, C_BLACK);
    Display.drawRect(GRID_X - 1, GRID_Y - 1,
                     COLS * CELL + 2, ROWS * CELL + 2, C_DARK_GREY);
    // Header background for score
    Display.fillRect(0, 0, SCREEN_W, 17, 0x0010);
}

// ─────────────────────────────────────────────────────────────
void Snake::_drawCell(uint8_t x, uint8_t y, uint16_t colour) {
    int16_t px = GRID_X + x * CELL;
    int16_t py = GRID_Y + y * CELL;
    if (colour == C_BLACK) {
        Display.fillRect(px, py, CELL, CELL, C_BLACK);
    } else {
        Display.fillRect(px + 1, py + 1, CELL - 2, CELL - 2, colour);
        Display.drawRect(px, py, CELL, CELL, C_BLACK);
    }
}

// ─────────────────────────────────────────────────────────────
void Snake::_drawFood() {
    int16_t px = GRID_X + _food.x * CELL + 1;
    int16_t py = GRID_Y + _food.y * CELL + 1;
    // Draw as a small red diamond
    Display.fillCircle(px + CELL/2 - 1, py + CELL/2 - 1, 2, C_RED);
}

// ─────────────────────────────────────────────────────────────
void Snake::_drawScore() {
    Display.fillRect(0, 0, SCREEN_W, 17, 0x0010);
    Display.setTextSize(1);
    Display.setTextColour(C_WHITE, 0x0010);
    Display.setCursor(2, 4);
    Display.print("SNAKE");

    char buf[20];
    snprintf(buf, sizeof(buf), "Score:%d Hi:%d", _score, _highScore);
    Display.setCursor(40, 4);
    Display.print(buf);
}

// ─────────────────────────────────────────────────────────────
void Snake::_drawGameOver() {
    // Dim the grid
    Display.fillRect(GRID_X, GRID_Y, COLS*CELL, ROWS*CELL, 0x2104);

    Display.printCentered("GAME OVER", 55, 2, C_RED, 0x2104);

    char buf[14];
    snprintf(buf, sizeof(buf), "Score: %d", _score);
    Display.printCentered(buf, 80, 1, C_WHITE, 0x2104);

    snprintf(buf, sizeof(buf), "Best:  %d", _highScore);
    Display.printCentered(buf, 92, 1, C_NOKIA_GREEN, 0x2104);

    Display.printCentered("OK=Restart", 110, 1, C_LIGHT_GREY, 0x2104);
    Display.printCentered("B=Menu",     122, 1, C_DARK_GREY,  0x2104);

    Serial.print(F("[SNAKE] Game over. Score: "));
    Serial.println(_score);
}
