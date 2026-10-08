// =============================================================
// Snake.h — Classic Snake Game
// =============================================================

#ifndef SNAKE_H
#define SNAKE_H

#include "../../core/AppManager.h"
#include "../../core/InputManager.h"

class Snake : public App {
public:
    void begin()  override;
    void update() override;
    void onInput(InputEvent ev) override;

private:
    static const uint8_t CELL     = 6;
    static const uint8_t COLS     = 20;   // 120 / 6
    static const uint8_t ROWS     = 21;   // 126 / 6
    static const uint8_t MAX_LEN  = COLS * ROWS;

    static const int16_t GRID_X   = 4;
    static const int16_t GRID_Y   = 18;

    enum Direction { DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT };
    enum GameState { STATE_START, STATE_PLAYING, STATE_DEAD };

    struct Point { uint8_t x, y; };

    Point     _body[MAX_LEN];
    uint16_t  _len;
    Direction _dir;
    Direction _nextDir;

    Point     _food;
    uint16_t  _score;
    uint16_t  _highScore;

    GameState _state;

    uint32_t  _lastMove;
    uint16_t  _speed;

    void _initGame();
    void _spawnFood();
    void _step();
    bool _collision(uint8_t x, uint8_t y, uint16_t skipTail = 0);

    void _drawStartScreen();
    void _drawGrid();
    void _drawCell(uint8_t x, uint8_t y, uint16_t colour);
    void _drawScore();
    void _drawGameOver();
    void _drawFood();
};

#endif // SNAKE_H
