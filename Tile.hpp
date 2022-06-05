#ifndef TILE_HPP
#define TILE_HPP

#include "DataStructures.hpp"

class Tile
{
    TileType m_tileType;
    double m_utility;
    double m_reward;
    ActionType m_actionType;

public:
    Tile();
    Tile(double reward, TileType tile = TileType::Normal);
    Tile(const Tile &tile);
    void SetTileType(TileType type);
    TileType GetTileType() const;
    void SetReward(double reward);
    double GetReward() const;
    void SetOptimalAction(ActionType action);
    ActionType GetOptimalAction() const;
    void SetUtility(double utility);
    double GetUtility() const;
    Tile &operator=(const Tile &tile);
};

#endif