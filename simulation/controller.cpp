#include "controller.hpp"
#include "creature.hpp"
#include "bodyParts.hpp"

AntWorkerAI::AntWorkerAI(Creature* owner):
    Controller(owner),
    panicTimeLeft_(0){
}

void AntWorkerAI::step(int deltatime){
    auto& antLegs=owner_->getAntLegs();
    auto& antMandibles=owner_->getAntMandibles();
    auto& antSensors=owner_->getAntSensors();
    auto& antWorkerAbdomens=owner_->getAntWorkerAbdomens();
    auto& antQueenAbdomens=owner_->getAntQueenAbdomens();

    if(antLegs.empty()){
        std::cout<<"Nie mam sensorow\n";
        return;
    }
    if(antMandibles.empty()){
        std::cout<<"Nie mam rzujek\n";
        return;
    }
    if(antSensors.empty()){
        std::cout<<"Nie mam nog\n";
        return;
    }
    if(antWorkerAbdomens.empty()){
        std::cout<<"Nie mam kaloryfera\n";
        return;
    }

    if((antLegs.size()>1) ||
        (antMandibles.size()>1) ||
        (antSensors.size()>1) ||
        (antQueenAbdomens.size()>0) ||
        (antWorkerAbdomens.size()>1)){
        std::cout<<"radiation\n";
        return;
    }
    
    auto legs = antLegs[0];
    auto ma = antMandibles[0];
    auto sensor = antSensors[0];
    auto abd = antWorkerAbdomens[0];

    // ant starts to panic when is probably deadlocked with other ants
    if(panicTimeLeft_>0){
        legs->goRandom();
        --panicTimeLeft_;
        return;
    }

    if(!ma->isHolding()){
        bool targetPosChanged=0;
        Point targetPos=Point(rand()%40+1,rand()%40+1);
            
        auto observations = sensor->getEntities();
        for(auto o : observations){
            if(sensor->getAnthillPheromoneStrength(o.getPos())>0.05)
                // this food is already in Anthill
                continue;

            // smell of food (enum ?)
            if((o.getSmell()==100) && !ma->isHolding()){
                if(o.getPos() != owner_->getPos()){
                    if(!targetPosChanged){
                        targetPos=o.getPos();
                        targetPosChanged=1; }
                    continue;
                }
                ma->grab(o);
                std::cout<<"grabbed\n";
                break;
            }
        }

        abd->dropToFoodPheromones();
        legs->goToPos(targetPos);
    }else{
        abd->dropFromFoodPheromones();

        Point target=sensor->getClosestAnthillPheromone(0);

        if(target!=Point(INT_MAX,INT_MAX)){
            if(target==owner_->getPos()){
                ma->drop();
            }else{
                legs->goToPos(target);
            }
        }else{
            // search Anthill
            legs->goRandom();
        }
    }

    if(legs->getTimeNotMoving()>3){
        panicTimeLeft_=5;
        legs->goRandom();
    }

}

AntQueenAI::AntQueenAI(Creature* owner):
    Controller(owner){
}

void AntQueenAI::step(int deltatime){
    auto& antLegs=owner_->getAntLegs();
    auto& antMandibles=owner_->getAntMandibles();
    auto& antSensors=owner_->getAntSensors();
    auto& antWorkerAbdomens=owner_->getAntWorkerAbdomens();
    auto& antQueenAbdomens=owner_->getAntQueenAbdomens();

    if(antLegs.empty()){
        std::cout<<"Nie mam sensorow\n";
        return;
    }
    if(antMandibles.empty()){
        std::cout<<"Nie mam rzujek\n";
        return;
    }
    if(antSensors.empty()){
        std::cout<<"Nie mam nog\n";
        return;
    }
    if(antQueenAbdomens.empty()){
        std::cout<<"Nie mam kaloryfera\n";
        return;
    }

    if((antLegs.size()>1) ||
        (antMandibles.size()>1) ||
        (antSensors.size()>1) ||
        (antWorkerAbdomens.size()>0) ||
        (antQueenAbdomens.size()>1)){
        std::cout<<"radiation\n";
        return;
    }

    auto legs = antLegs[0];
    auto ma = antMandibles[0];
    auto sensor = antSensors[0];
    auto abd = antQueenAbdomens[0];


    // simply leave anthill pheromones so that 
    // ants know they are at home
    if(sensor->getAnthillPheromoneStrength(owner_->getPos()) < 2){
        abd->dropAnthillPheromones();
    }
}
