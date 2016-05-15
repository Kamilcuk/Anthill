#include "creature.hpp"

#include "world.hpp"
#include "ant.hpp"

float Creature::bite(float strength){
    int ret=std::min(strength,energy_);
    energy_-=strength;
    energy_=std::max(energy_,0.0f);
    return ret;
}

