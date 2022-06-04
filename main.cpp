#include "Map.hpp"

int main()
{
    Map map;
    map.ReadMapFromFile("/home/jacek/Desktop/programming/markov/map1.markov");
    map.PrintWorld();
    return 0;
}