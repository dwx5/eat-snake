#pragma once
#include <vector>
enum class Direction {
    UP,    // 上
    DOWN,  // 下
    LEFT,  // 左
    RIGHT  // 右
};
// 坐标结构体，表示游戏中的位置
struct Point {
    int x;  // 横向坐标（网格单位）
    int y;  // 纵向坐标（网格单位）
    Point(int x_ = 0, int y_ = 0) : x(x_), y(y_) {}
};
class Snake{
    private:

    public:

};