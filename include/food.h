#ifndef FOOD_H
#define FOOD_H

#include "Pos.h"
#include <vector>

class Food {
private:
    std::vector<Pos> positions;  // 存储多个食物的位置

public:
    // 构造函数：初始生成指定数量的食物
    Food(int initialCount = 3);  // 默认生成3个食物

    // 获取所有食物的位置
    const std::vector<Pos>& getAllPos() const;

    // 清除所有食物
    void clear();

    // 重新生成多个食物（参数：数量，蛇身位置）
    void regenerateMultiple(int count, const std::vector<Pos>& snakeBody);

    // 移除指定位置的食物（返回是否成功移除）
    bool remove(const Pos& pos);
};

#endif // FOOD_H
    