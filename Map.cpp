#include "Map.hpp"
#include <fstream>
#include <iostream>

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
    std::ifstream file;
    file.open(filePath);
    char command;
    if (!file)
    {
        std::cerr << "Error: file could not be opened" << std::endl;
        exit(1);
    }

    file >> command;
    if (command != 'W')
    {
        std::cerr << "No W label in world file" << std::endl;
        throw std::runtime_error("No W label");
    }
    else
    {
        file >> m_sizeX >> m_sizeY;
        m_map = new Tile *[m_sizeX];
        for (int i = 0; i < m_sizeX; i++)
        {
            m_map[i] = new Tile[m_sizeY];
        }
        file >> command;
    }

    if (command == 'S')
    {
        int start_x, start_y;
        file >> start_x >> start_y;
        m_startPosition.SetPosition(start_x - 1, start_y - 1);
        m_agentPosition.SetPosition(start_x - 1, start_y - 1);
        m_map[start_x - 1][start_y - 1].SetTileType(TileType::Start);
        file >> command;
    }
    else
    {
        m_startPosition.SetPosition(0, 0);
        m_agentPosition.SetPosition(0, 0);
        m_map[0][0].SetTileType(TileType::Start);
    }

    if (command != 'P')
    {
        std::cerr << "No P label in world file" << std::endl;
        throw std::runtime_error("No P label");
    }
    else
    {
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
        file >> command;
    }

    if (command != 'R')
    {
        std::cerr << "No R label in world file" << std::endl;
        throw std::runtime_error("No R label");
    }
    else
    {
        file >> m_defaultReward;
        for (int i = 0; i < m_sizeX; i++)
        {
            for (int j = 0; j < m_sizeY; j++)
            {
                m_map[i][j].SetReward(m_defaultReward);
            }
        }
        file >> command;
    }

    if (command == 'G')
    {
        file >> m_discountingParameter;
        file >> command;
    }
    else
    {
        m_discountingParameter = 1;
    }

    if (command == 'E')
    {
        file >> m_explorationParameter;
        file >> command;
    }
    else
    {
        m_explorationParameter = 1;
    }

    if (command != 'T')
    {
        std::cerr << "No T label in world file" << std::endl;
        throw std::runtime_error("No T label");
    }
    else
    {
        int x, y;
        float reward;
        file >> x >> y >> reward;
        m_map[x-1][y-1].SetTileType(TileType::Terminal);
        m_map[x-1][y-1].SetReward(reward);
        file >> command;
    }

    while ((command == 'T' || command == 'B') && !file.eof())
    {
        int x, y;
        float reward;
        file >> x >> y >> reward;
        m_map[x-1][y-1].SetReward(reward);
        if (command == 'T')
        {
            m_map[x-1][y-1].SetTileType(TileType::Terminal);
        }
        else
        {
            m_map[x-1][y-1].SetTileType(TileType::Special);
        }
        file >> command;
    }

    while (command == 'F' && !file.eof())
    {
        int x, y;
        file >> x >> y;
        m_map[x-1][y-1].SetTileType(TileType::Prohibitet);
        file >> command;
    }

    file.close();
}

// Position Map::GetAgentsPosition();

void Map::PrintWorld(){
    for(int j=m_sizeY-1; j>=0; j--){
        std::cout << std::string(7 * m_sizeX + 1, '_') << std::endl;
        std::cout << "|";
        for(int i = 0; i < m_sizeX; i++){
            switch(m_map[i][j].GetTileType()){
                case TileType::Prohibitet: 
                std::cout << std::string(6, 'F');
            }
        }
    }
}