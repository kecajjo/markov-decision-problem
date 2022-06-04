#ifndef MAP_HPP
#define MAP_HPP

#include "Position.hpp"
#include "Tile.hpp"
#include <utility>
#include <string>
#include <map>

class Map
{
    int m_sizeX;
    int m_sizeY;
    double m_defaultReward;
    double m_discountingParameter;
    double m_explorationParameter;
    Tile **m_map = nullptr;
    Position m_agentPosition;
    Position m_startPosition;
    std::map<char, double> m_moveChances;//f - forward, l - left, r - right, b - backward

public:
    Map();
    ~Map();
    void ReadMapFromFile(std::string filePath);
    //Position GetAgentsPosition();
    void PrintWorld();
    
};

#endif