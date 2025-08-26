#include "Pos.h"  // 包含头文件

// 实现构造函数
Pos::Pos(int x, int y) : x(x), y(y) {}

// 实现相等性运算符重载
bool Pos::operator==(const Pos& other) const {
    return x == other.x && y == other.y;
}
    