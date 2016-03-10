# file for testing visualisation
# this file is treated as library written in c++ for anthill simulation

from timer import Timer

class World:
    # private:  not required in library
    def _nextFrame(self):
        for a in self.ants:
            a.x+=1
            a.y+=1


    # public:
    class Ant:
        x=0
        y=0
        rot=0

        def getX(self):
            return self.x

        def getY(self):
            return self.y

        def getRotation(self):
            return self.rot

    def __init__(self):
        self.ants=[]

        a=World.Ant()
        a.x=10
        a.y=20
        self.ants+=[a]

        b=World.Ant()
        b.x=50
        b.y=50
        self.ants+=[b]

        self.w=0
        self.h=0

        self.timer=None
        self.framerate=1

    def getAnts(self):
        return self.ants


    def setDimensions(self,w,h):
        self.w=w
        self.h=h

    def setSimulationFramerate(self,f):
        self.framerate=f
        pass

    def startSimulation(self):
        if self.timer is None:
            self.timer=Timer(self._nextFrame,self.framerate)
            self.timer.start()

    def stopSimulation(self):
        if self.timer:
            self.timer.stop()
            self.timer=None
