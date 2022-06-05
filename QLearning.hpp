#ifndef Q_LEARNING_HPP
#define Q_LEARNING_HPP

#include "Map.hpp"

class QLearning
{

    unsigned int m_iterations;
    double m_discountingParameter;
    double m_explorationParameter;
    Map m_map;

    // Position m_prevPos;
    // TileType m_prevPosType;
    // ActionType m_prevAction;
    // double prevReward;
    std::map<std::tuple<int, int, ActionType>, double> Q;//utility of action type in position
    std::map<std::tuple<int, int, ActionType>, int> N;//how many times action type was perfromed from position

public:
    QLearning(std::string mapFilePath, unsigned int iterations, double discounting = -1, double exploration = -1);
    ActionType Explore(ActionType optimalAction);
    void Execute();
    std::pair<double, ActionType> GetMaxQ(int x, int y);
};

#endif