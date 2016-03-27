#include "antcontroller.h"

#include <iostream>
#include <cstdlib>
#include "anthill.hpp"
#include "pheromoneMap.hpp"
#include "food.hpp"
#include <typeinfo>
#include "world.hpp"
#include "ant.hpp"
#include "entity.hpp"
#include <algorithm>

AntController::AntController()
{

}

#define dupaprint(str,e) std::cout<<str" type: "<<typeid(*e).name()<<" "<<e<<"pos: "; e->getPos().printPosition();

void AntController::control(Ant *controlled)
{
    /** how to get to other ants? */
    /** how to get to Pheromones */
    /** how to increment pos to chase them? */
    /** so many questions */
    Point goThere(-1,-1);

    std::vector<std::shared_ptr<Updatable>> ups = controlled->getWorld().getUpdatables();

    /* controlled is bad */
    std::vector<std::shared_ptr<PheromoneMap>> pheromoneMaps = controlled->getWorld().getDerivedUpdatable<PheromoneMap>();
    std::shared_ptr<PheromoneMap> pheroFromFood;
    std::shared_ptr<PheromoneMap> pheroToFood;
    for(auto& p : pheromoneMaps) {
        if ( p->getType() == PheromoneMap::ToFood ) {
            pheroToFood = p;
        }
        if ( p->getType() == PheromoneMap::FromFood ) {
            pheroFromFood = p;
        }
    }



    dupaprint("Mrowka",controlled);
    if ( auto lifted = controlled->getLifted().lock() ) {
        dupaprint("^ carrying",lifted);
    }


    if ( !controlled->isLifting() ) {
        // we are not carrying enything
        // if we seed food we go there
        std::map<std::shared_ptr<Food>,float> visibleFoods = controlled->getVisibleWithDistances<Food>();
        // get all food, that are not carried
        std::map<std::shared_ptr<Food>,float> visibleFreeFoods;
        for(auto& i : visibleFoods) {
            if ( !i.first->isCarried() ) {
                visibleFreeFoods.insert(i);
            }
        }
        // well, more then one visible food ?
        if ( visibleFreeFoods.size() > 0 ) {
            // closest visible free food
            auto closestVisibleFood = std::min_element(visibleFreeFoods.begin(), visibleFreeFoods.end(),
                                                       [&](std::map<std::shared_ptr<Food>,float>::value_type &f1,
                                                       std::map<std::shared_ptr<Food>,float>::value_type &f2){return f1.second<f2.second;} );
            std::cout << "Distance to closest food: " << closestVisibleFood->second << std::endl;
            if ( closestVisibleFood->second == 0 ) {
                // we are standing on food - lets pick it up
                controlled->lift( closestVisibleFood->first );
            } else {
                // we see food -> lets go there
                goThere = closestVisibleFood->first->getPos();

                std::cout << "phero to food2" << std::endl;
                pheroToFood->createBlob(controlled->getPos(), 2, 100);
            }
        }
    } else {
        // we are carrying food
        // lets go to AntQueen!
        auto visibleQueens = controlled->getVisibleWithDistances<Anthill>();
        if ( visibleQueens.size() > 0 ) {
            auto closestVisibleAntQueen = std::min_element(visibleQueens.begin(), visibleQueens.end());
            if ( closestVisibleAntQueen->second == 0 ) {
                // we are standing on AntQueen and having food - lets give it her!
                std::weak_ptr<Entity> f = controlled->getLifted();
                controlled->unLift();
                closestVisibleAntQueen->first->lift(f);
            } else {
                // we have food, and see antqueen
                std::cout << "pheroFromFood2" << std::endl;
                pheroFromFood->createBlob(controlled->getPos(), 2, 100);

                goThere = closestVisibleAntQueen->first->getPos();
            }
        }
    }

    if ( goThere.posX() == -1 && goThere.posY() == -1 ) {
        Point strongestPheromone(-1,-1);
        float pheroStrength = 0;
        if ( controlled->isLifting() ) {
            strongestPheromone = pheroFromFood->getStrongestAtArea(controlled->getPos(), controlled->getVisibilityRadius());
            if ( strongestPheromone.posX() > 0 && strongestPheromone.posY() > 0 )
                pheroStrength = pheroFromFood->getStrengthAtPosition(strongestPheromone);
        } else {
            strongestPheromone = pheroToFood->getStrongestAtArea(controlled->getPos(), controlled->getVisibilityRadius());
            if ( strongestPheromone.posX() > 0 && strongestPheromone.posY() > 0 )
                pheroStrength = pheroToFood->getStrengthAtPosition(strongestPheromone);
        }
        if ( pheroStrength > 20 || strongestPheromone.posX() == -1 || strongestPheromone.posY() == -1 ) {
            std::cout << " strongestPheromone " << pheroStrength << std::endl;
            goThere = strongestPheromone;
        } else {
            /* aa! no where to go? go somewhere random! */
            std::cout << " random " << std::endl;
            goThere = controlled->getPos().move(rand()%2 ? -1 : 1,rand()%2 ? -1 : 1);
        }
    }
    if ( goThere.posX() < 0 || goThere.posY() < 0 ) {
        goThere =  Point(30,30);
    }

        /* calculate next position */
        int t, addX=-1, addY=-1;
        std::cout << " ^ GoThere: "; goThere.printPosition();
        t = controlled->getPos().posX() - goThere.posX();
        addX = ( t == 0 ? 0 : t < 0 ? +1 : -1 ) * controlled->getSpeed();
        t = controlled->getPos().posY() - goThere.posY();
        addY = ( t == 0 ? 0 : t < 0 ? +1 : -1 ) * controlled->getSpeed();
        Point newPos = controlled->getPos().move(addX, addY);

        /* set position */
        controlled->setPos( newPos );
        std::cout << "^ idz do: addX: " << addX << " addY: " << addY << std::endl;

        std::cout << std::endl;
}

void AntController::control(Entity *controlled)
{
    return control(dynamic_cast<Ant*>(controlled));
}
