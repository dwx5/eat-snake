#ifndef AUTOSNAKE_H
#define AUTOSNAKE_H

#include "Pos.h"
#include <vector>

class AutoSnake {
private:
    std::vector<Pos> body; 
    int dir;       
    
public:
    AutoSnake();
    Pos getHead() const;               
    const std::vector<Pos>& getBody() const;        
    bool move(const std::vector<Pos>& playerSnakeBody);                      
    void grow();
    
    // 新增方法
    void changeDir(int newDir);
    int getDirection() const;
    void updateAI(const std::vector<Pos>& allFoodPos, const std::vector<Pos>& obstacles);
    
};

#endif