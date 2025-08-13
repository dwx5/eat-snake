#include "Game.h"
#include "Pos.h"
#include <iostream>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <ctime>

// 颜色定义（ANSI码）
#define ANSI_RESET "\033[0m"
#define ANSI_RED "\033[31m"
#define ANSI_GREEN "\033[32m"
#define ANSI_YELLOW "\033[33m"
#define ANSI_WHITE "\033[37m"
#define ANSI_CYAN "\033[36m"

// 游戏初始化
Game::Game() : snake(), food(3), score(0), running(true), paused(false) {
    srand(time(0));
    hideCursor();
    snake.changeDir(4);  // 初始向右自动移动
}

Game::~Game() {
    showCursor();
}

// 非阻塞键盘输入检测
int Game::kbhit() {
    struct termios oldt, newt;
    int ch, oldf;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);  // 关闭缓冲和回显
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);  // 非阻塞模式
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);  // 恢复终端设置
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
}

// 隐藏光标
void Game::hideCursor() {
    std::cout << "\033[?25l";
}

// 显示光标
void Game::showCursor() {
    std::cout << "\033[?25h";
}

// 新增：移动光标到终端左上角（实现单画面刷新）
void Game::moveCursorToTop() {
    std::cout << "\033[0;0H";  // ANSI控制码：定位到(0,0)坐标
}

// 绘制游戏界面（单画面刷新版本）
void Game::draw() {
    moveCursorToTop();  // 关键：移动光标到顶部覆盖刷新（替代清屏）

    // 上边框
    std::cout << ANSI_WHITE << "┌";
    for (int i = 0; i < 18; i++) std::cout << "─";
    std::cout << "┐" << std::endl;

    // 获取所有食物位置
    const std::vector<Pos>& allFoodPos = food.getAllPos();

    // 游戏区域（逐行绘制）
    for (int y = 0; y < 15; y++) {
        for (int x = 0; x < 20; x++) {
            if (x == 0 || x == 19) {  // 左右边框
                std::cout << ANSI_WHITE << "│";
            } else if (x == snake.getHead().x && y == snake.getHead().y) {  // 蛇头
                std::cout << ANSI_YELLOW << "■";
            } else {
                bool isBody = false;
                // 检测蛇身
                for (const auto& p : snake.getBody()) {
                    if (x == p.x && y == p.y) {
                        std::cout << ANSI_GREEN << "■";
                        isBody = true;
                        break;
                    }
                }
                if (!isBody) {
                    // 检测食物
                    bool isFood = false;
                    for (const auto& fp : allFoodPos) {
                        if (x == fp.x && y == fp.y) {
                            std::cout << ANSI_RED << "■";
                            isFood = true;
                            break;
                        }
                    }
                    if (!isFood) {
                        std::cout << " ";  // 空白区域
                    }
                }
            }
        }
        std::cout << std::endl;
    }

    // 下边框
    std::cout << ANSI_WHITE << "└";
    for (int i = 0; i < 18; i++) std::cout << "─";
    std::cout << "┘" << std::endl;

    // 信息栏：分数和长度
    std::cout << ANSI_CYAN 
              << "分数: " << score 
              << " | 长度: " << snake.getLength() 
              << " | 控制: ↑↓←→ | 空格: 暂停 | Q: 退出" 
              << ANSI_RESET << std::endl;
              
    // 新增：覆盖暂停提示（避免残留）
    if (paused) {
        std::cout << "=== 游戏暂停中，按空格继续 ===" << std::endl;
    } else {
        std::cout << "                                       " << std::endl;  // 空白覆盖
    }
}

// 处理用户输入
void Game::handleInput() {
    if (kbhit()) {
        char c = getchar();
        if (c == '\033') {  // 方向键
            getchar();
            switch (getchar()) {
                case 'A': snake.changeDir(1); break;  // 上
                case 'B': snake.changeDir(2); break;  // 下
                case 'D': snake.changeDir(3); break;  // 左
                case 'C': snake.changeDir(4); break;  // 右
            }
        } else if (c == ' ') {  // 空格暂停
            paused = !paused;
        } else if (c == 'q' || c == 'Q') {  // 退出
            running = false;
        }
    }
}

// 更新游戏逻辑
void Game::update() {
    if (paused) return;

    // 蛇移动并检测碰撞
    if (snake.move()) {
        running = false;
        return;
    }

    // 检测吃食物
    Pos snakeHead = snake.getHead();
    const std::vector<Pos>& allFoodPos = food.getAllPos();
    bool ateFood = false;

    for (const auto& fp : allFoodPos) {
        if (snakeHead == fp) {
            score += 10;
            snake.grow();
            food.remove(fp);
            food.regenerateMultiple(1, snake.getBody());
            ateFood = true;
            break;
        }
    }
}

// 游戏主循环（单画面刷新+低速移动）
void Game::run() {
    draw();  // 初始绘制（无清屏）
    while (running) {
        handleInput();
        update();
        draw();  // 覆盖刷新画面
        usleep(400000);  // 降低速度：300ms/步（数值越大越慢）
    }

    // 游戏结束界面
    std::cout << "\033[2J\033[1;1H";  // 结束时清屏
    std::cout << ANSI_RED << "=== 游戏结束 ===" << std::endl;
    std::cout << "最终分数: " << score 
              << " | 最终长度: " << snake.getLength() << ANSI_RESET << std::endl;
    std::cout << "按任意键退出..." << std::endl;
    getchar();
}

    
