#include "QLearning.hpp"
#include <string>

QLearning::QLearning(std::string mapFilePath, unsigned int iterations, double discounting, double exploration){
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
}

ActionType QLearning::Explore(ActionType optimalAction)
{
    auto randomAction = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    if (randomAction > m_explorationParameter)
    {
        return optimalAction;
    }
    else
    {
        randomAction = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        if (randomAction < 0.25)
        {
            return ActionType::Up;
        }
        else if (randomAction < 0.5)
        {
            return ActionType::Right;
        }
        else if (randomAction < 0.75)
        {
            return ActionType::Down;
        }
        else
        {
            return ActionType::Left;
        }
    }
    return ActionType::Unknown;
}

void QLearning::Execute()
{
    m_map.InitSaveMapResults();
    for (int i = 0; i < m_iterations; ++i)
    {
        m_map.RestartPos();
        while (true)
        {
            Position agentPos = m_map.GetAgentPosition();
            int x, y;
            std::tie(x, y) = agentPos.GetPosition();
            if (m_map.GetTile(x, y).GetTileType() == TileType::Terminal)
            {
                break;
            }

            ActionType currentAction = Explore(m_map.GetOptimalActionForTile(x, y));
            if (N.find(std::make_tuple(x, y, currentAction)) == N.end())
            {
                N[std::make_tuple(x, y, currentAction)] = 1;
            }
            else
            {
                N[std::make_tuple(x, y, currentAction)] += 1;
            }

            double QVal = 0;
            if (Q.find(std::make_tuple(x, y, currentAction)) == Q.end()){
                Q[std::make_tuple(x, y, currentAction)] = 0;
            } else{
                QVal = Q[std::make_tuple(x, y, currentAction)];
            }

            m_map.Move(currentAction);

            Position newPos = m_map.GetAgentPosition();
            int newX, newY;
            std::tie(newX, newY) = newPos.GetPosition();
            if (Q.find(std::make_tuple(newX, newY, currentAction)) == Q.end()){
                Q[std::make_tuple(newX, newY, currentAction)] = 0;
            }

            double alpha = 1.0 / N[std::make_tuple(x, y, currentAction)];

            ActionType actionMaxQ;
            double maxQ;

            std::tie(maxQ, actionMaxQ) = GetMaxQ(newX, newY);

            if (m_map.GetTile(newX, newY).GetTileType() == TileType::Terminal)
            {
                maxQ = m_map.GetReward(newX, newY);
            }

            double reward = m_map.GetReward(newX,newY);
            double newQ = reward + m_discountingParameter * maxQ;
            Q[std::make_tuple(x, y, currentAction)] += alpha * (newQ - Q[std::make_tuple(x, y, currentAction)]);
            m_map.SetOptimalActionForTile(newX, newY, actionMaxQ);

            ActionType actionForMaxQinCurState;
            double maxQForCurPos;
            std::tie(maxQForCurPos, actionForMaxQinCurState) = GetMaxQ(x, y);
            m_map.SetUtilityOfTile(x, y, maxQForCurPos);
            m_map.SetOptimalActionForTile(x, y, actionForMaxQinCurState);
            m_map.PrintWorld();
        }
        if (i % 50 == 0)
        {
            m_map.SaveMapResults(i);
        }
    }
    m_map.PrintWorld();
}

std::pair<double, ActionType> QLearning::GetMaxQ(int x, int y){
    double maxQ = -1000000;
    double q;
    ActionType retAction = ActionType::Up;
    if (Q.find(std::make_tuple(x, y, ActionType::Up)) != Q.end()){
        q = N[std::make_tuple(x, y, ActionType::Up)];
        if(maxQ < q){
            maxQ = q;
            retAction = ActionType::Up;
        }
    }
    if (Q.find(std::make_tuple(x, y, ActionType::Right)) != Q.end()){
        q = N[std::make_tuple(x, y, ActionType::Right)];
        if(maxQ < q){
            maxQ = q;
            retAction = ActionType::Right;
        }
    }
    if (Q.find(std::make_tuple(x, y, ActionType::Down)) != Q.end()){
        q = N[std::make_tuple(x, y, ActionType::Down)];
        if(maxQ < q){
            maxQ = q;
            retAction = ActionType::Down;
        }
    }
    if (Q.find(std::make_tuple(x, y, ActionType::Left)) != Q.end()){
        q = N[std::make_tuple(x, y, ActionType::Left)];
        if(maxQ < q){
            maxQ = q;
            retAction = ActionType::Left;
        }
    }
    return {maxQ, retAction};
}