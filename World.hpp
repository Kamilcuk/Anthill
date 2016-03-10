#pragma once

#include <vector>
#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

class World{
    int width,height;
    float framerate;
    private:
        void mainLoop();

    public:
    //shared_ptr<std::vector<shared_ptr<Ant> > > getAnts();

    void setDimensions(int,int);
    void setSimulationFramerate(float);

    void startSimulation();
    void stopSimulation();
};
