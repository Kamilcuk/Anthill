#ifndef CONTROLLER_H_
#define CONTROLLER_H_

class Creature;

class Controller{
protected:
    Creature* owner_;
public:
    Controller(Creature* owner){
        owner_=owner;
    }
    virtual void step(int deltaTime)=0;
};

class AntWorkerAI : public Controller{
public:
    AntWorkerAI(Creature* owner);
    void step(int deltaTime);
};

#endif
