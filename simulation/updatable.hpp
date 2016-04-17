#ifndef UPDATABLE_H
#define UPDATABLE_H

#include <boost/enable_shared_from_this.hpp>
class World;

class Updatable : public boost::enable_shared_from_this<Updatable> {
protected:
    World& world_;
    bool alive = true;
public:
    Updatable(World &world);
    virtual ~Updatable();
	virtual void step(int) = 0;
    World &getWorld();

    void remove();
    bool isAlive() const;
};

#endif // UPDATABLE_H
