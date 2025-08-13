#include "Snake.h"
#include "Pos.h"

// 蛇初始位置：中间3节身体，方向向上
Snake::Snake() {
    body.push_back(Pos(10, 7));   // 蛇头（假设WIDTH=20, HEIGHT=15，中间位置）
    body.push_back(Pos(10, 8));
    body.push_back(Pos(10, 9));
    dir = 1;  // 初始向上
}

// 获取蛇头（身体第一个元素）
Pos Snake::getHead() const {
    return body.front();
}

// 获取蛇身
const std::vector<Pos>& Snake::getBody() const {
    return body;
}

// 改变方向（禁止反向移动，如向上时不能直接向下）
void Snake::changeDir(int newDir) {
    if ((dir == 1 && newDir != 2) || 
        (dir == 2 && newDir != 1) || 
        (dir == 3 && newDir != 4) || 
        (dir == 4 && newDir != 3)) {
        dir = newDir;
    }
}

// 移动蛇：返回是否碰撞（撞墙或自身）
bool Snake::move() {
    Pos head = body.front();  // 当前蛇头位置

    // 计算新蛇头位置
    switch (dir) {
        case 1: head.y--; break;  // 上
        case 2: head.y++; break;  // 下
        case 3: head.x--; break;  // 左
        case 4: head.x++; break;  // 右
    }

    // 撞墙检测（WIDTH=20, HEIGHT=15，边框范围）
    if (head.x <= 0 || head.x >= 19 || head.y < 0 || head.y >= 15) {
        return true;
    }

    // 撞自身检测
    for (const auto& p : body) {
        if (head.x == p.x && head.y == p.y) {
            return true;
        }
    }

    // 移动逻辑：新头加入，尾部删除（保持长度）
    body.insert(body.begin(), head);
    body.pop_back();
    return false;
}

// 蛇生长：新头加入，不删尾部（长度+1）
void Snake::grow() {
    Pos head = body.front();
    switch (dir) {
        case 1: body.insert(body.begin(), Pos(head.x, head.y - 1)); break;
        case 2: body.insert(body.begin(), Pos(head.x, head.y + 1)); break;
        case 3: body.insert(body.begin(), Pos(head.x - 1, head.y)); break;
        case 4: body.insert(body.begin(), Pos(head.x + 1, head.y)); break;
    }
}
