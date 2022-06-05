#ifndef POSITION_HPP
#define POSITION_HPP

#include <utility>
#include "DataStructures.hpp"

class Position
{
    int m_x;
    int m_y;

public:
    Position(int x = 0, int y = 0);
    void SetPosition(int x, int y);
    std::pair<int, int> GetPosition() const;
    void Move(ActionType action);
    Position &operator=(const Position &pos);
    bool operator==(const Position &pos) const;
};

#endif