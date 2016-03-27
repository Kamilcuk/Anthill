#include "simulation/world.hpp"

int main() {
    World world;
    world.startSimulation();
    for(unsigned int i=0;i<100000;++i) {
        world.simulationStep();
    }
    return 0;
}
