// =============================================================
// Snake.cpp — Classic Snake Game Implementation
// =============================================================

#include "Snake.h"
#include "../../core/DisplayManager.h"
#include "../../core/AppManager.h"
#include "../../storage/StorageManager.h"

void Snake::begin() {
    Serial.println(F("[APP] Snake started"));
    _highScore = Storage.getSnakeHighScore();
    _state     = STATE_START;
    _drawStartScreen();
}

void Snake::_initGame() {
    _score     = 0;
    _speed     = 220;
    _state     = STATE_PLAYING;

    _len = 3;
    _body[0] = { 10, 10 };  // Head
    _body[1] = { 9, 10 };
    _body[2] = { 8, 10 };
    _dir     = DIR_RIGHT;
    _nextDir = DIR_RIGHT;

    randomSeed(micros());
    _spawnFood();

    _lastMove = millis();

    Display.clear(C_BLACK);
    _drawGrid();
    _drawScore();

    for (uint16_t i = 0; i < _len; i++) {
        uint16_t c = (i == 0) ? C_NOKIA_GREEN : 0x0600;
        _drawCell(_body[i].x, _body[i].y, c);
    }
    _drawFood();
}

void Snake::update() {
    if (_state != STATE_PLAYING) return;

    uint32_t now = millis();
    if (now - _lastMove < _speed) return;
    _lastMove = now;

    _dir = _nextDir;
    _step();
}

void Snake::onInput(InputEvent ev) {
    if (_state == STATE_START) {
        if (ev == INPUT_OK || ev == INPUT_KEY_5) {
            _initGame();
        } else if (ev == INPUT_BACK || ev == INPUT_STAR) {
            AppMgr.launchApp(APP_MENU);
        } else if (ev == INPUT_HOME || ev == INPUT_KEY_0) {
            AppMgr.launchApp(APP_LAUNCHER);
        }
        return;
    }

    if (_state == STATE_DEAD) {
        if (ev == INPUT_OK || ev == INPUT_KEY_5) {
            _initGame();
        } else if (ev == INPUT_BACK || ev == INPUT_STAR) {
            AppMgr.launchApp(APP_MENU);
        } else if (ev == INPUT_HOME || ev == INPUT_KEY_0) {
            AppMgr.launchApp(APP_LAUNCHER);
        }
        return;
    }

    // STATE_PLAYING controls
    switch (ev) {
        case INPUT_UP:
        case INPUT_KEY_2:
            if (_dir != DIR_DOWN)  _nextDir = DIR_UP;
            break;

        case INPUT_DOWN:
        case INPUT_KEY_8:
            if (_dir != DIR_UP)    _nextDir = DIR_DOWN;
            break;

        case INPUT_LEFT:
        case INPUT_KEY_4:
            if (_dir != DIR_RIGHT) _nextDir = DIR_LEFT;
            break;

        case INPUT_RIGHT:
        case INPUT_KEY_6:
            if (_dir != DIR_LEFT)  _nextDir = DIR_RIGHT;
            break;

        case INPUT_BACK:
        case INPUT_STAR:
            AppMgr.launchApp(APP_MENU);
            break;

        case INPUT_HOME:
        case INPUT_KEY_0:
            AppMgr.launchApp(APP_LAUNCHER);
            break;

        default:
            break;
    }
}

void Snake::_step() {
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

    // Self collision
    if (_collision((uint8_t)nx, (uint8_t)ny, 1)) {
        _state = STATE_DEAD;
        _drawGameOver();
        return;
    }

    bool ate = (nx == _food.x && ny == _food.y);

    if (!ate) {
        _drawCell(_body[_len - 1].x, _body[_len - 1].y, C_BLACK);
        for (uint16_t i = _len - 1; i > 0; i--) {
            _body[i] = _body[i - 1];
        }
    } else {
        if (_len < MAX_LEN) {
            for (uint16_t i = _len; i > 0; i--) {
                _body[i] = _body[i - 1];
            }
            _len++;
        }
    }

    _body[0] = { (uint8_t)nx, (uint8_t)ny };

    if (_len > 1) {
        _drawCell(_body[1].x, _body[1].y, 0x0600);
    }
    _drawCell(_body[0].x, _body[0].y, C_NOKIA_GREEN);

    if (ate) {
        _score++;
        if (_score > _highScore) {
            _highScore = _score;
            Storage.setSnakeHighScore(_highScore);
        }

        // Increase speed (minimum 60ms)
        if (_speed > 60 && _score % 3 == 0) {
            _speed -= 15;
        }

        _spawnFood();
        _drawFood();
        _drawScore();
    }
}

bool Snake::_collision(uint8_t x, uint8_t y, uint16_t skipTail) {
    uint16_t limit = (skipTail && _len > 0) ? _len - 1 : _len;
    for (uint16_t i = 0; i < limit; i++) {
        if (_body[i].x == x && _body[i].y == y) return true;
    }
    return false;
}

void Snake::_spawnFood() {
    uint8_t fx, fy;
    do {
        fx = (uint8_t)(random(COLS));
        fy = (uint8_t)(random(ROWS));
    } while (_collision(fx, fy, 0));
    _food = { fx, fy };
}

void Snake::_drawStartScreen() {
    Display.clear(C_BLACK);
    Display.drawHeader("SNAKE", C_NOKIA_BLUE, C_WHITE);

    // Decorative retro frame
    Display.drawRect(8, 22, SCREEN_W - 16, 110, C_NOKIA_BLUE);

    Display.printCentered("RETRO SNAKE", 32, 1, C_NOKIA_GREEN, C_BLACK);

    // Mini decorative snake art
    int sx = 44, sy = 52;
    Display.fillRect(sx, sy, 8, 8, C_NOKIA_GREEN);
    Display.fillRect(sx + 9, sy, 8, 8, 0x0600);
    Display.fillRect(sx + 18, sy, 8, 8, 0x0600);
    Display.fillRect(sx + 27, sy, 8, 8, 0x0600);
    Display.fillCircle(sx + 42, sy + 4, 3, C_RED);

    char hiBuf[24];
    snprintf(hiBuf, sizeof(hiBuf), "Best Score: %u", _highScore);
    Display.printCentered(hiBuf, 72, 1, C_YELLOW, C_BLACK);

    Display.printCentered("2,8,4,6 : Steer", 92, 1, C_WHITE, C_BLACK);
    Display.printCentered("Press 5 to Play", 110, 1, C_NOKIA_CYAN, C_BLACK);

    Display.drawSoftKeys("*: Menu", "5: Start", C_NOKIA_BLUE, C_WHITE);
}

void Snake::_drawGrid() {
    Display.drawRect(GRID_X - 1, GRID_Y - 1, COLS * CELL + 2, ROWS * CELL + 2, C_DARK_GREY);
}

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

void Snake::_drawFood() {
    int16_t px = GRID_X + _food.x * CELL + 1;
    int16_t py = GRID_Y + _food.y * CELL + 1;
    Display.fillCircle(px + CELL / 2 - 1, py + CELL / 2 - 1, 2, C_RED);
}

void Snake::_drawScore() {
    Display.fillRect(0, 0, SCREEN_W, 16, C_NOKIA_BLUE);
    Display.setTextSize(1);
    Display.setTextColour(C_WHITE, C_NOKIA_BLUE);

    char buf[28];
    snprintf(buf, sizeof(buf), "Score:%u  Hi:%u", _score, _highScore);
    Display.setCursor(4, 4);
    Display.print(buf);
}

void Snake::_drawGameOver() {
    Display.fillRect(GRID_X + 6, GRID_Y + 20, COLS * CELL - 12, 80, C_BLACK);
    Display.drawRect(GRID_X + 6, GRID_Y + 20, COLS * CELL - 12, 80, C_RED);

    Display.printCentered("GAME OVER", GRID_Y + 30, 2, C_RED, C_BLACK);

    char buf[20];
    snprintf(buf, sizeof(buf), "Score: %u", _score);
    Display.printCentered(buf, GRID_Y + 54, 1, C_WHITE, C_BLACK);

    snprintf(buf, sizeof(buf), "Best:  %u", _highScore);
    Display.printCentered(buf, GRID_Y + 66, 1, C_YELLOW, C_BLACK);

    Display.printCentered("5: Restart   *: Menu", GRID_Y + 82, 1, C_LIGHT_GREY, C_BLACK);

    Display.drawSoftKeys("*: Menu", "5: Restart", C_NOKIA_BLUE, C_WHITE);
}
