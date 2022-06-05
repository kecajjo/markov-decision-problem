#ifndef MAP_HPP
#define MAP_HPP

#include "Position.hpp"
#include "Tile.hpp"
#include <utility>
#include <string>
#include <map>
#include <vector>

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
    std::map<char, double> m_moveChances; //f - forward, l - left, r - right, b - backward

public:
    Map();
    ~Map();
    void ReadMapFromFile(std::string filePath);
    double GetDiscountingParameter() const;
    double GetExplorationParameter() const;
    double GetDefaultReward() const;
    double GetReward(int x, int y) const;
    void SetOptimalActionForTile(int x, int y, ActionType action);
    ActionType GetOptimalActionForTile(int x, int y) const;
    int GetSizeX() const;
    int GetSizeY() const;
    double GetUtilityOfTile(int x, int y) const;
    void SetUtilityOfTile(int x, int y, double utility);
    //returns probablity of ending in certain position after performig action from position
    std::vector<std::pair<Position, double>> GetActionPossibleResults(ActionType action, Position pos);
    bool IsPositionAllowed(Position pos) const;
    bool IsPositionAllowed(int x, int y) const;
    //Position GetAgentsPosition();
    void PrintWorld() const;
    std::map<char, double> GetMoveChances() const;
    Map &operator=(const Map &map);
    Tile GetTile(int x, int y) const;
    void SetTile(int x, int y, const Tile &tile);
    void InitSaveMapResults();
    void SaveMapResults(int iteration);
};

#endif