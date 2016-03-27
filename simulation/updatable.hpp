#ifndef UPDATABLE_H
#define UPDATABLE_H

#include <memory>
class World;

class Updatable : public std::enable_shared_from_this<Updatable> {
    World& world_;
    bool alive = true;
public:
    Updatable(World &world);
    virtual ~Updatable();
	virtual void step() = 0;
    World &getWorld();

    void remove();
    bool isAlive() const;
};

#endif // UPDATABLE_H
