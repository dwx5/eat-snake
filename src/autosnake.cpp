#include "Autosnake.h"
#include "Pos.h"
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <vector>

// 自动蛇初始位置：右上角，方向向左
AutoSnake::AutoSnake() {
    body.push_back(Pos(18, 2));   // 蛇头
    body.push_back(Pos(19, 2));   // 蛇身
    body.push_back(Pos(19, 3));   // 蛇身
    dir = 3;  // 初始向左移动
}

Pos AutoSnake::getHead() const {
    return body.front();
}

const std::vector<Pos>& AutoSnake::getBody() const {
    return body;
}

int AutoSnake::getDirection() const {
    return dir;
}

void AutoSnake::changeDir(int newDir) {
    // 禁止反向移动
    if ((dir == 1 && newDir != 2) || 
        (dir == 2 && newDir != 1) || 
        (dir == 3 && newDir != 4) || 
        (dir == 4 && newDir != 3)) {
        dir = newDir;
    }
}

void AutoSnake::grow() {
    Pos head = body.front();
    switch (dir) {
        case 1: body.insert(body.begin(), Pos(head.x, head.y - 1)); break;
        case 2: body.insert(body.begin(), Pos(head.x, head.y + 1)); break;
        case 3: body.insert(body.begin(), Pos(head.x - 1, head.y)); break;
        case 4: body.insert(body.begin(), Pos(head.x + 1, head.y)); break;
    }
}

bool AutoSnake::move(const std::vector<Pos>& playerSnakeBody) {
    Pos head = body.front();

    // 计算新蛇头位置
    switch (dir) {
        case 1: head.y--; break;  // 上
        case 2: head.y++; break;  // 下
        case 3: head.x--; break;  // 左
        case 4: head.x++; break;  // 右
    }

    // 撞墙检测（避开边框）
    if (head.x <= 1 || head.x >= 18 || head.y < 1 || head.y >= 14) {
        return true;
    }

    // 撞自身检测
    for (const auto& p : body) {
        if (head.x == p.x && head.y == p.y) {
            return true;
        }
    }
    //撞玩家蛇检测
    for (const auto& p : playerSnakeBody) {
        if (head.x == p.x && head.y == p.y) {
            return true;
        }
    }


    // 移动逻辑
    body.insert(body.begin(), head);
    body.pop_back();
    return false;
}

// AI决策：自动寻找食物并避开障碍物
void AutoSnake::updateAI(const std::vector<Pos>& allFoodPos, const std::vector<Pos>& obstacles) {
    if (allFoodPos.empty()) return;
    
    Pos head = getHead();
    
    // 1. 找到最近的食物
    Pos closestFood = allFoodPos[0];
    double minDistance = 1000.0;
    
    for (const auto& food : allFoodPos) {
        double dist = std::abs(food.x - head.x) + std::abs(food.y - head.y);
        if (dist < minDistance) {
            minDistance = dist;
            closestFood = food;
        }
    }
    
    // 2. 计算方向偏好（朝向食物）
    int preferredDir = dir; // 默认保持当前方向
    
    if (std::abs(closestFood.x - head.x) > std::abs(closestFood.y - head.y)) {
        // 水平距离更大，优先水平移动
        if (closestFood.x > head.x) preferredDir = 4; // 右
        else if (closestFood.x < head.x) preferredDir = 3; // 左
    } else {
        // 垂直距离更大，优先垂直移动
        if (closestFood.y > head.y) preferredDir = 2; // 下
        else if (closestFood.y < head.y) preferredDir = 1; // 上
    }
    
    // 3. 检查首选方向是否安全
    Pos testPos = head;
    switch (preferredDir) {
        case 1: testPos.y--; break;
        case 2: testPos.y++; break;
        case 3: testPos.x--; break;
        case 4: testPos.x++; break;
    }
    
    bool isSafe = true;
    
    // 检查墙壁
    if (testPos.x <= 2 || testPos.x >= 17 || testPos.y < 2 || testPos.y >= 13) {
        isSafe = false;
    }
    
    // 检查障碍物
    for (const auto& obs : obstacles) {
        if (testPos.x == obs.x && testPos.y == obs.y) {
            isSafe = false;
            break;
        }
    }
    
    // 检查自身（除了尾部）
    for (size_t i = 0; i < body.size() - 1; i++) {
        if (testPos.x == body[i].x && testPos.y == body[i].y) {
            isSafe = false;
            break;
        }
    }
    
    // 4. 如果首选方向安全，就选择它
    if (isSafe) {
        changeDir(preferredDir);
        return;
    }
    
    // 5. 如果不安全，尝试其他方向
    std::vector<int> possibleDirs;
    std::vector<Pos> testPositions = {
        Pos(head.x, head.y - 1), // 上
        Pos(head.x, head.y + 1), // 下
        Pos(head.x - 1, head.y), // 左
        Pos(head.x + 1, head.y)  // 右
    };
    
    for (int i = 0; i < 4; i++) {
        if ((dir == 1 && i == 1) || (dir == 2 && i == 0) || 
            (dir == 3 && i == 3) || (dir == 4 && i == 2)) {
            continue; // 跳过反向移动
        }
        
        Pos test = testPositions[i];
        bool safe = true;
        
        // 检查墙壁
        if (test.x <= 2 || test.x >= 17 || test.y < 2 || test.y >= 13) {
            safe = false;
        }
        
        // 检查障碍物
        for (const auto& obs : obstacles) {
            if (test.x == obs.x && test.y == obs.y) {
                safe = false;
                break;
            }
        }
        
        // 检查自身
        for (size_t j = 0; j < body.size() - 1; j++) {
            if (test.x == body[j].x && test.y == body[j].y) {
                safe = false;
                break;
            }
        }
        
        if (safe) {
            possibleDirs.push_back(i + 1); // 方向编号从1开始
        }
    }
    
    // 6. 如果有安全方向，随机选择一个
    if (!possibleDirs.empty()) {
        int randomIndex = rand() % possibleDirs.size();
        changeDir(possibleDirs[randomIndex]);
    }
    // 如果没有安全方向，保持原方向（可能会撞死）
}
