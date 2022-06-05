#include "QLearning.hpp"

int main(int argc, char *argv[])
{
    QLearning *qLearn;
    double discounting, exploration;
    if(argc == 1)
        qLearn = new QLearning("../map.markov", 10000);
    else if (argc ==2){
        sscanf(argv[1],"%lf",&discounting);
        qLearn = new QLearning("../map.markov", 10000, discounting);
    } else{
        sscanf(argv[1],"%lf",&discounting);
        sscanf(argv[2],"%lf",&exploration);
        qLearn = new QLearning("../map.markov", 10000, discounting, exploration);
    }

    qLearn->Execute();
    return 0;
}