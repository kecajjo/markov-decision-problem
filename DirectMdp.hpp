#ifndef DIRECT_MDP_HPP
#define DIRECT_MDP_HPP

#include "Map.hpp"

class DirectMdp
{
    Map m_map;
    Map m_prevMap;
    unsigned int m_iterations;
    double m_discountingParameter;
    double m_explorationParameter;

    Tile CalculateMdpForTile(int x, int y);

public:
    DirectMdp(std::string mapFilePath, unsigned int iterations, double discounting = -1, double exploration = -1);
    void Execute();

};

#endif