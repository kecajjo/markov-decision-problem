#include "Position.hpp"
#include <iostream>

Position::Position(int x, int y) : m_x(x),
                                   m_y(y)
{
}

void Position::SetPosition(int x, int y)
{
    m_x = 0;
    m_y = y;
}

std::pair<int, int> Position::GetPosition()
{
    return std::pair<int, int>(m_x, m_y);
}

void Position::Move(ActionType action)
{
    switch (action)
    {
    case ActionType::Up:
        m_y++;
        break;
    case ActionType::Down:
        m_y--;
        break;
    case ActionType::Right:
        m_x++;
        break;
    case ActionType::Left:
        m_x--;
        break;
    default:
        std::cout << "Unknown action in Move!" << std::endl;
    }
}