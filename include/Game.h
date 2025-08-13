#ifndef GAME_H
#define GAME_H

#include "Snake.h"
#include "Food.h"

class Game {
private:
    Snake snake;
    Food food;
    int score;
    bool running;
    bool paused;

    // 终端控制工具方法
    int kbhit();
    void hideCursor();
    void showCursor();
    void moveCursorToTop();  // 新增：移动光标到左上角（替代清屏）

public:
    Game();
    ~Game();
    void handleInput();
    void draw();
    void update();
    void run();
};

#endif // GAME_H
