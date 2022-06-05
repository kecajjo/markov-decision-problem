project(MarkovDecisionProblem)

set(CMAKE_BUILD_TYPE Debug)

add_executable(DirectMarkov 
    DirectMarkovMain.cpp
    Tile.cpp
    Position.cpp
    Map.cpp
    DirectMdp.cpp
)