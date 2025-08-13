#ifndef SNAKE_H
#define SNAKE_H

#include "Pos.h"
#include <vector>

// 蛇类：管理蛇的身体、方向、移动和生长
class Snake {
private:
    std::vector<Pos> body;  // 蛇身坐标
    int dir;                // 方向：1上 2下 3左 4右

public:
    Snake();
    Pos getHead() const;                // 获取蛇头位置
    const std::vector<Pos>& getBody() const;  // 获取蛇身
    void changeDir(int newDir);         // 改变移动方向（禁止反向）
    bool move();                        // 移动蛇（返回是否碰撞）
    void grow();                        // 蛇生长（吃到食物时）
    // 添加：获取蛇的长度（包含头部）
    int getLength() const {
        return body.size();  // 蛇的长度就是身体容器的大小
    }
};

#endif // SNAKE_H
