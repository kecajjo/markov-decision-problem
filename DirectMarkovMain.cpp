#include "DirectMdp.hpp"

int main(int argc, char *argv[])
{
    DirectMdp *mdp;
    double discounting, exploration;
    if(argc == 1)
        mdp = new DirectMdp("../map.markov", 10000);
    else if (argc ==2){
        sscanf(argv[1],"%lf",&discounting);
        mdp = new DirectMdp("../map.markov", 10000, discounting);
    } else{
        sscanf(argv[1],"%lf",&discounting);
        sscanf(argv[2],"%lf",&exploration);
        mdp = new DirectMdp("../map.markov", 10000, discounting, exploration);
    }

    mdp->Execute();
    return 0;
}