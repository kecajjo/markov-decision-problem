#include "Map.hpp"
#include <fstream>
#include <iostream>
#include <iomanip>

Map::Map()
{
}

Map::~Map()
{
    if (m_map != nullptr)
    {
        for (int i = 0; i < m_sizeX; i++)
        {
            delete[] m_map[i];
        }
        delete[] m_map;
    }
}

void Map::ReadMapFromFile(std::string filePath)
{
    //default params
    m_explorationParameter = 1;
    m_discountingParameter = 1;
    bool w = false, s = false, p = false, r = false, g = false, e = false, t = false;

    std::ifstream file;
    file.open(filePath);
    char command;
    if (!file)
    {
        std::cerr << "Error: file could not be opened" << std::endl;
        exit(1);
    }

    file >> command;
    while (!file.eof())
    {
        switch (command)
        {
        case 'W':
            if (w)
            {
                std::cerr << "Second W label in world file" << std::endl;
                throw std::runtime_error("Second W label");
            }
            file >> m_sizeX >> m_sizeY;
            m_map = new Tile *[m_sizeX];
            for (int i = 0; i < m_sizeX; i++)
            {
                m_map[i] = new Tile[m_sizeY];
            }
            w = true;
            break;

        case 'S':
        {
            if (s)
            {
                std::cerr << "Second S label in world file" << std::endl;
                throw std::runtime_error("Second S label");
            }
            int start_x, start_y;
            file >> start_x >> start_y;
            m_startPosition.SetPosition(start_x - 1, start_y - 1);
            m_agentPosition.SetPosition(start_x - 1, start_y - 1);
            m_map[start_x - 1][start_y - 1].SetTileType(TileType::Start);
            s = true;
        }
        break;

        case 'P':
        {
            if (p)
            {
                std::cerr << "Second P label in world file" << std::endl;
                throw std::runtime_error("Second P label");
            }
            double p1, p2, p3;
            file >> p1 >> p2 >> p3;
            m_moveChances['f'] = p1;
            m_moveChances['l'] = p2;
            m_moveChances['r'] = p3;
            if (1 - p1 - p2 - p3 < 0.0000001)
            {
                m_moveChances['b'] = 0;
            }
            else
            {
                m_moveChances['b'] = 1 - p1 - p2 - p3;
            }
            p = true;
        }
        break;

        case 'R':
        {
            if (r)
            {
                std::cerr << "Second R label in world file" << std::endl;
                throw std::runtime_error("Second R label");
            }
            file >> m_defaultReward;
            for (int i = 0; i < m_sizeX; i++)
            {
                for (int j = 0; j < m_sizeY; j++)
                {
                    m_map[i][j].SetReward(m_defaultReward);
                }
            }
            r = true;
        }
        break;

        case 'G':
            if (g)
            {
                std::cerr << "Second G label in world file" << std::endl;
                throw std::runtime_error("Second G label");
            }
            file >> m_discountingParameter;
            g = true;
            break;

        case 'E':
            if (e)
            {
                std::cerr << "Second E label in world file" << std::endl;
                throw std::runtime_error("Second E label");
            }
            file >> m_explorationParameter;
            e = true;
            break;

        case 'T':
        {
            int x, y;
            float reward;
            file >> x >> y >> reward;
            m_map[x - 1][y - 1].SetTileType(TileType::Terminal);
            m_map[x - 1][y - 1].SetReward(reward);
            t = true;
        }
        break;

        case 'B':
        {
            int x, y;
            float reward;
            file >> x >> y >> reward;
            m_map[x - 1][y - 1].SetReward(reward);
            m_map[x - 1][y - 1].SetTileType(TileType::Special);
        }
        break;

        case 'F':
        {
            int x, y;
            file >> x >> y;
            m_map[x - 1][y - 1].SetTileType(TileType::Prohibitet);
        }
        break;
        }
        file >> command;
    }
    file.close();
    if (!w || !p || !r || !t)
    {
        std::cerr << "Missing mandatory label in the world file" << std::endl;
        throw std::runtime_error("Missing mandatory label in the world file");
    }
}

double Map::GetDiscountingParameter() const
{
    return m_discountingParameter;
}

double Map::GetExplorationParameter() const
{
    return m_explorationParameter;
}

double Map::GetReward(int x, int y) const
{
    return m_map[x][y].GetReward();
}

void Map::SetOptimalActionForTile(int x, int y, ActionType action)
{
    m_map[x][y].SetOptimalAction(action);
}

ActionType Map::GetOptimalActionForTile(int x, int y) const
{
    return m_map[x][y].GetOptimalAction();
}

int Map::GetSizeX() const
{
    return m_sizeX;
}

int Map::GetSizeY() const
{
    return m_sizeY;
}

double Map::GetUtilityOfTile(int x, int y) const
{
    if(m_map[x][y].GetTileType() == TileType::Terminal){
        return m_map[x][y].GetReward();    
    }
    return m_map[x][y].GetUtility();
}

void Map::SetUtilityOfTile(int x, int y, double utility)
{
    m_map[x][y].SetUtility(utility);
}

std::vector<std::pair<Position, double>> Map::GetActionPossibleResults(ActionType action, Position pos)
{
    std::vector<std::pair<Position, double>> retVal;
    std::vector<std::pair<ActionType, double>> movesWithProbability;
    switch (action)
    {
    case ActionType::Up:
        movesWithProbability.push_back({ActionType::Up, m_moveChances['f']});
        movesWithProbability.push_back({ActionType::Left, m_moveChances['l']});
        movesWithProbability.push_back({ActionType::Right, m_moveChances['r']});
        movesWithProbability.push_back({ActionType::Down, m_moveChances['b']});
        break;
    case ActionType::Right:
        movesWithProbability.push_back({ActionType::Right, m_moveChances['f']});
        movesWithProbability.push_back({ActionType::Up, m_moveChances['l']});
        movesWithProbability.push_back({ActionType::Down, m_moveChances['r']});
        movesWithProbability.push_back({ActionType::Left, m_moveChances['b']});
        break;
    case ActionType::Down:
        movesWithProbability.push_back({ActionType::Down, m_moveChances['f']});
        movesWithProbability.push_back({ActionType::Right, m_moveChances['l']});
        movesWithProbability.push_back({ActionType::Left, m_moveChances['r']});
        movesWithProbability.push_back({ActionType::Up, m_moveChances['b']});
        break;
    case ActionType::Left:
        movesWithProbability.push_back({ActionType::Left, m_moveChances['f']});
        movesWithProbability.push_back({ActionType::Down, m_moveChances['l']});
        movesWithProbability.push_back({ActionType::Up, m_moveChances['r']});
        movesWithProbability.push_back({ActionType::Right, m_moveChances['b']});
        break;
    }
    for (int i = 0; i < movesWithProbability.size(); i++)
    {
        Position tmpPos = pos;
        tmpPos.Move(movesWithProbability[i].first);
        if (IsPositionAllowed(tmpPos))
        {
            retVal.push_back({tmpPos, movesWithProbability[i].second});
        }
        else
        { //didnt move
            retVal.push_back({pos, movesWithProbability[i].second});
        }
    }
    for (int i = 0; i < retVal.size(); i++)
    {
        for (int j = i + 1; j < retVal.size(); j++)
        {
            if (retVal[i].first == retVal[j].first)
            {
                //if final position is the same add probabilities of both moves and delete second
                retVal[i].second = retVal[i].second + retVal[j].second;
                retVal.erase(retVal.begin() + j);
                j--; //decrease j as curent element was removed
            }
        }
        if (retVal[i].second < 0.000001)
        {
            retVal.erase(retVal.begin() + i);
            i--;
        }
    }
    return retVal;
}

bool Map::IsPositionAllowed(Position pos) const
{
    int x, y;
    std::tie(x, y) = pos.GetPosition();
    return IsPositionAllowed(x, y);
}

bool Map::IsPositionAllowed(int x, int y) const
{
    if (x < 0 || y < 0 || x >= m_sizeX || y >= m_sizeY)
    {
        return false;
    }
    if (m_map[x][y].GetTileType() == TileType::Prohibitet)
    {
        return false;
    }
    return true;
}

void Map::PrintWorld() const
{
    for (int j = m_sizeY - 1; j >= 0; j--)
    {
        std::cout << std::string(7 * m_sizeX + 1, '_') << std::endl;
        std::cout << "|";
        for (int i = 0; i < m_sizeX; i++)
        {
            switch (m_map[i][j].GetTileType())
            {
            case TileType::Prohibitet:
                std::cout << std::string(6, 'F');
                break;
            case TileType::Terminal:
                std::cout << std::string(6, 'T');
                break;
            default:
                std::cout << std::fixed;
                std::cout << std::setprecision(4);
                if(m_map[i][j].GetUtility() > 0){
                std::cout << m_map[i][j].GetUtility();
                } else{
                    std::cout << "      ";
                }
            }
            std::cout << "|";
        }
        std::cout << std::endl
                  << "|";

        for (int i = 0; i < m_sizeX; i++)
        {
            switch (m_map[i][j].GetTileType())
            {
            case TileType::Prohibitet:
                std::cout << std::string(6, 'F');
                break;
            default:
                std::cout << std::fixed;
                if (m_map[i][j].GetReward() > 0)
                {
                    std::cout << " ";
                }
                std::cout << std::setprecision(3);
                std::cout << m_map[i][j].GetReward();
            }
            std::cout << "|";
        }
        std::cout << std::endl
                  << "|";

        for (int i = 0; i < m_sizeX; i++)
        {
            switch (m_map[i][j].GetTileType())
            {
            case TileType::Prohibitet:
                std::cout << std::string(6, 'F');
                break;
            case TileType::Terminal:
                std::cout << std::string(6, 'T');
                break;
            default:
            {
                char c;
                if (m_map[i][j].GetOptimalAction() == ActionType::Up)
                {
                    c = '^';
                }
                else if (m_map[i][j].GetOptimalAction() == ActionType::Right)
                {
                    c = '>';
                }
                else if (m_map[i][j].GetOptimalAction() == ActionType::Down)
                {
                    c = 'v';
                }
                else if (m_map[i][j].GetOptimalAction() == ActionType::Left)
                {
                    c = '<';
                }
                else
                {
                    c = ' ';
                }
                std::cout << std::fixed;

                std::cout << "  " << c << "   ";
            }
            }
            std::cout << "|";
        }
        std::cout << std::endl;
    }
    std::cout << std::string(7 * m_sizeX + 1, '_') << std::endl;
    std::cout << std::endl;
}

std::map<char, double> Map::GetMoveChances() const
{
    return m_moveChances;
}

double Map::GetDefaultReward() const
{
    return m_defaultReward;
}

Map &Map::operator=(const Map &map)
{
    if (m_sizeX != map.GetSizeX() || m_sizeY != map.GetSizeY())
    {
        if (m_map != nullptr)
        {
            for (int i = 0; i < m_sizeX; i++)
            {
                delete[] m_map[i];
            }
            delete[] m_map;
            m_map = nullptr;
        }
    }
    m_sizeX = map.GetSizeX();
    m_sizeY = map.GetSizeY();
    m_defaultReward = map.GetDefaultReward();
    m_discountingParameter = map.GetDiscountingParameter();
    m_explorationParameter = map.GetExplorationParameter();
    if (m_map == nullptr)
    {
        m_map = new Tile *[m_sizeX];
        for (int i = 0; i < m_sizeX; i++)
        {
            m_map[i] = new Tile[m_sizeY];
        }
    }
    for (int i = 0; i < m_sizeX; i++)
    {
        for (int j = 0; j < m_sizeY; j++)
        {
            m_map[i][j] = map.GetTile(i, j);
            if (map.GetTile(i, j).GetTileType() == TileType::Start)
            {
                m_startPosition.SetPosition(i, j);
            }
        }
    }
    // m_agentPosition;
    m_moveChances = map.GetMoveChances();
}

Tile Map::GetTile(int x, int y) const
{
    return m_map[x][y];
}

void Map::SetTile(int x, int y, const Tile &tile){
    m_map[x][y] = tile;
}