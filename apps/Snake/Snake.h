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
    // Grid configuration
    // Play area: x=2..125, y=18..147  (122×130 px)
    // Cell size: 6px → 20 cols × 21 rows
    static const uint8_t CELL     = 6;
    static const uint8_t COLS     = 20;   // 120 / 6
    static const uint8_t ROWS     = 21;   // 126 / 6
    static const uint8_t MAX_LEN  = COLS * ROWS;

    static const int16_t GRID_X   = 3;
    static const int16_t GRID_Y   = 19;

    enum Direction { DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT };
    enum GameState { STATE_PLAYING, STATE_DEAD };

    // Snake body stored as flat (col, row) pairs
    struct Point { uint8_t x, y; };

    Point     _body[MAX_LEN];
    uint16_t  _len;
    Direction _dir;
    Direction _nextDir;  // Buffered input direction

    Point     _food;
    uint16_t  _score;
    uint16_t  _highScore;

    GameState _state;

    uint32_t  _lastMove;
    uint16_t  _speed;   // ms per step

    void _spawnFood();
    void _step();
    bool _collision(uint8_t x, uint8_t y, uint16_t skipTail = 0);

    void _drawGrid();
    void _drawCell(uint8_t x, uint8_t y, uint16_t colour);
    void _drawScore();
    void _drawGameOver();
    void _drawFood();
    void _flashHead();
};

#endif // SNAKE_H
