#ifndef TILE_HPP
#define TILE_HPP

#include "DataStructures.hpp"

class Tile{
    TileType m_tileType;
    double m_utility;
    double m_reward;
    ActionType m_actionType;
public:
    Tile(double reward, TileType tile = TileType::Normal);
    TileType GetTileType();
    double GetReward();
    void SetOptimalAction(ActionType action);
    ActionType GetOptimalAction();
    void SetUtility(double utility);
    double GetUtility();
};

#endif