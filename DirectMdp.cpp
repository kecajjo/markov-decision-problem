#include "DirectMdp.hpp"

DirectMdp::DirectMdp(std::string mapFilePath, unsigned int iterations, double discounting, double exploration)
{
    m_map.ReadMapFromFile(mapFilePath);
    m_iterations = iterations;
    m_discountingParameter = discounting;
    m_explorationParameter = exploration;
    if (m_discountingParameter < 0)
    {
        m_discountingParameter = m_map.GetDiscountingParameter();
    }
    if (m_explorationParameter < 0)
    {
        m_explorationParameter = m_map.GetExplorationParameter();
    }
    m_prevMap = m_map;
}

Tile DirectMdp::CalculateMdpForTile(int x, int y)
{

    //Dont calculate utility for terminal state
    if (m_prevMap.GetTile(x, y).GetTileType() == TileType::Terminal)
    {
        return m_prevMap.GetTile(x, y);
    }
    Tile retVal = m_prevMap.GetTile(x, y);
    double rewardForPos = m_prevMap.GetReward(x, y);
    double maxUtil = -1;
    Position pos(x, y);

    for (int i = 0; i < static_cast<int>(ActionType::Unknown); i++)
    {
        auto possibleResults = m_map.GetActionPossibleResults(static_cast<ActionType>(i), pos);
        double utilOfAction = 0;
        for (int j = 0; j < possibleResults.size(); j++)
        {
            int tmpX, tmpY;
            std::tie(tmpX, tmpY) = possibleResults[j].first.GetPosition();
            utilOfAction += m_prevMap.GetUtilityOfTile(tmpX, tmpY) * possibleResults[j].second;
        }
        if (utilOfAction > maxUtil)
        {
            maxUtil = utilOfAction;
            retVal.SetOptimalAction(static_cast<ActionType>(i));
        }
    }

    double calculatedUtil = rewardForPos + m_discountingParameter * maxUtil;
    retVal.SetUtility(calculatedUtil);
    return retVal;
}

void DirectMdp::Execute()
{
    for (int iteration = 0; iteration < m_iterations; iteration++)
    {
        for (int i = 0; i < m_map.GetSizeX(); i++)
        {
            for (int j = 0; j < m_map.GetSizeY(); j++)
            {
                if (m_map.GetTile(i, j).GetTileType() != TileType::Prohibitet && m_map.GetTile(i, j).GetTileType() != TileType::Terminal)
                {
                    m_map.SetTile(i, j, CalculateMdpForTile(i, j));
                }
            }
        }
        m_prevMap = m_map;
    }
    m_map.PrintWorld();
}