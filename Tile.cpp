#include "Tile.hpp"

Tile::Tile() : m_reward(0),
               m_tileType(TileType::Normal),
               m_utility(0),
               m_actionType(ActionType::Unknown)
{
}

Tile::Tile(double reward, TileType tile) : m_reward(reward),
                                           m_tileType(tile),
                                           m_utility(0),
                                           m_actionType(ActionType::Unknown)
{
}

void Tile::SetTileType(TileType type)
{
    m_tileType = type;
}

TileType Tile::GetTileType()
{
    return m_tileType;
}

void Tile::SetReward(double reward){
    m_reward = reward;
}

double Tile::GetReward()
{
    return m_reward;
}

void Tile::SetOptimalAction(ActionType action)
{
    m_actionType = action;
}

ActionType Tile::GetOptimalAction()
{
    return m_actionType;
}

void Tile::SetUtility(double utility)
{
    m_utility = utility;
}

double Tile::GetUtility()
{
    return m_utility;
}