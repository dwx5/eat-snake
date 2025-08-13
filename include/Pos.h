#ifndef POS_H
#define POS_H

// 坐标类：存储x、y位置，提供相等性判断
class Pos {
public:
    int x, y;
    Pos(int x = 0, int y = 0);
    bool operator==(const Pos& other) const;
};

#endif // POS_H
