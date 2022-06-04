#ifndef POSITION_HPP
#define POSITION_HPP

#include <pair>
#include "DataStructures.hpp"

class Position{
    m_x;
    m_y;
public:
    Position(int x=0, int y=0);
    void SetPosition(int x, int y);
    std::pair<int,int> GetPosition();
    void Move(ActionType action);
};

#endif