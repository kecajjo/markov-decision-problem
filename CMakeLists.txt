project(MarkovDecisionProblem)

set(CMAKE_BUILD_TYPE Debug)

add_executable(DirectMarkov 
    DirectMarkovMain.cpp
    Tile.cpp
    Position.cpp
    Map.cpp
    DirectMdp.cpp
)

add_executable(QLearning
    QLearningMain.cpp
    Tile.cpp
    Position.cpp
    Map.cpp
    QLearning.cpp
)