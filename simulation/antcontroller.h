#ifndef ANTCONTROLLER_H
#define ANTCONTROLLER_H

#include "controller.h"

class Ant;
class Entity;
class AntController : public Controller
{
public:
    AntController();

    // Controller interface
public:
    void control(Ant *controlled);
    void control(Entity *controlled);
};

#endif // ANTCONTROLLER_H
