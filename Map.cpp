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

// Position Map::GetAgentsPosition();

void Map::PrintWorld()
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
                std::cout << m_map[i][j].GetUtility();
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