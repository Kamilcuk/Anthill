#ifndef CONTROLLER_H
#define CONTROLLER_H

class Entity;
class Controller
{
public:
    Controller();
    virtual void control(Entity *e) = 0;
};

#endif // CONTROLLER_H
