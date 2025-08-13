#include "Food.h"
#include "Pos.h"
#include <cstdlib>  // 随机数
#include <vector>

// 初始生成多个食物
Food::Food(int initialCount) {
    // 生成指定数量的初始食物（避开空蛇身）
    regenerateMultiple(initialCount, {});
}

// 获取所有食物位置
const std::vector<Pos>& Food::getAllPos() const {
    return positions;
}

// 清除所有食物
void Food::clear() {
    positions.clear();
}

// 重新生成多个食物（避开蛇身）
void Food::regenerateMultiple(int count, const std::vector<Pos>& snakeBody) {
    positions.clear();  // 先清空现有食物
    
    for (int i = 0; i < count; ++i) {
        bool valid;
        Pos newPos;
        
        do {
            valid = true;
            // 随机位置：x(1~18)，y(1~13)（避开边框）
            newPos = Pos(rand() % 18 + 1, rand() % 13 + 1);
            
            // 检查是否与蛇身重叠
            for (const auto& p : snakeBody) {
                if (newPos.x == p.x && newPos.y == p.y) {
                    valid = false;
                    break;
                }
            }
            
            // 检查是否与已生成的食物重叠
            if (valid) {
                for (const auto& p : positions) {
                    if (newPos.x == p.x && newPos.y == p.y) {
                        valid = false;
                        break;
                    }
                }
            }
        } while (!valid);  // 直到生成有效位置
        
        positions.push_back(newPos);
    }
}

// 移除指定位置的食物（吃到食物时调用）
bool Food::remove(const Pos& pos) {
    for (auto it = positions.begin(); it != positions.end(); ++it) {
        if (it->x == pos.x && it->y == pos.y) {
            positions.erase(it);
            return true;  // 成功移除
        }
    }
    return false;  // 未找到该食物
}
    