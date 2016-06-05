#include "controller.hpp"
#include "creature.hpp"
#include "bodyParts.hpp"
#include "pheromoneMap.hpp"

std::pair<bool,bool> Controller::eatingActivity(
        boost::shared_ptr<AntLegs> legs,
        boost::shared_ptr<AntSensor> sensor, 
        boost::shared_ptr<AntMandibles> ma){
    bool eatenSomething=false;
    bool foundTarget=false;
    auto observations = sensor->getObservations();
    for(auto o : observations){
        if((o.getSmell()==Entity::Smell::Food)){
            if(ma->bite(o)==false){
                if(foundTarget)
                    continue;
                Point eatingPos=o.getPos();
                if(sensor->isAccessible(o) || o.getPos()==owner_->getPos()){
                    legs->goToPos(eatingPos);
                    foundTarget=true;
                }
            }else{
                foundTarget=true;
                eatenSomething=true;
                break;
            }
        }
    }
    return std::make_pair(foundTarget,eatenSomething);
}

std::pair<bool,bool> Controller::returnToAnthill(
        boost::shared_ptr<AntLegs> legs,
        boost::shared_ptr<AntSensor> sensor, 
        boost::shared_ptr<AntMandibles> ma){
    Point target=sensor->getClosestPheromone(PheromoneMap::Type::Anthill);
    bool lostTrace=false;
    bool returned=false;

    if(target!=Point(INT_MAX,INT_MAX)){
        // sensed Anthill
        if(target==owner_->getPos()){
            ma->drop();

            returned=true;

        }else{
            legs->goToPos(target);
        }
    }else{
        // search Anthill
        target=sensor->getFarthestPheromone(PheromoneMap::Type::ToFood);
        if(target!=owner_->getPos()){
            legs->goToPos(target);
        }else{
            lostTrace=true;
            // lost trace
            legs->goRandom();
        }
    }

    return std::make_pair(returned,lostTrace);
}

std::pair<bool,bool> Controller::goToFood(
        boost::shared_ptr<AntLegs> legs,
        boost::shared_ptr<AntSensor> sensor, 
        boost::shared_ptr<AntMandibles> ma){
    bool takenFood=false;
    bool usingFromFoodPheromones=false;
    Point target=sensor->getFarthestPheromone(PheromoneMap::Type::FromFood);
    if(target==owner_->getPos()){ // || timeSearchingWithFromFoodPheromones_>70){
        legs->goRandom();
    }else{
        legs->goToPos(target);
        usingFromFoodPheromones=true;
    }
        
    auto observations = sensor->getObservations();
    for(auto o : observations){
        if(sensor->getPheromoneStrength(PheromoneMap::Type::Anthill,o.getPos())>0.05)
            // this food is already in Anthill
            continue;

        if((o.getSmell()==Entity::Smell::Food) && !ma->isHolding()){
            if(o.getPos() != owner_->getPos()){ 
                if(sensor->isAccessible(o)){
                    //timeSearchingWithFromFoodPheromones_=0;
                    legs->goToPos(o.getPos());
                    break;
                }
            }else{
                ma->grab(o);
                takenFood=true;
                break;
            }
        }
    }

    return std::make_pair(takenFood,usingFromFoodPheromones);
}


bool Controller::biteAdjacentObstacle(
        boost::shared_ptr<AntSensor> sensor,
        boost::shared_ptr<AntMandibles> ma){
    for(auto o : sensor->getObservations()){
        if(owner_->getPos().isAdjacent(o.getPos())
                && o.getSmell()==Entity::Smell::None){
            if(ma->bite(o)){
                return true;
            }else{
                std::cout<<"strange"<<std::endl;
            }
        }
    }
    return false;
}

AntWorkerAI::AntWorkerAI(Creature* owner):
    Controller(owner),
    panicTimeLeft_(0),
    timeSearchingWithFromFoodPheromones_(0){
    currentActivity_=Activity::GoingToFoodSource;
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

    //if(!ma->isHolding()){
    if(currentActivity_==Activity::GoingToFoodSource){

        auto temp=goToFood(legs,sensor,ma);
        bool takenFood=temp.first;
        bool usingFromFoodPheromones=temp.second;

        if(usingFromFoodPheromones)
            ++timeSearchingWithFromFoodPheromones_;

        // maybe you are marking path to food for too long, 
        // this is probably a wrong path or cycle
        if(usingFromFoodPheromones && timeSearchingWithFromFoodPheromones_<150)
            abd->dropToFoodPheromones();

        if(takenFood){
            timeSearchingWithFromFoodPheromones_=0;
            abd->dropFromFoodPheromones();
            currentActivity_=Activity::TakingFoodToAnthill;
        }

        if(owner_->getEnergy()< 50)
            currentActivity_=Activity::Eating;

    }else if(currentActivity_==Activity::TakingFoodToAnthill){
        
        auto temp=returnToAnthill(legs,sensor,ma);
        bool returned=temp.first;
        bool lostTrace=temp.second;

        if(!lostTrace)
            abd->dropFromFoodPheromones();
        else
            legs->goRandom();

        if(returned){
            abd->dropFromFoodPheromones();

            if(owner_->getEnergy()< 50)
                currentActivity_=Activity::Eating;
            else
                currentActivity_=Activity::GoingToFoodSource;
        }else{
            if(owner_->getEnergy()< 50)
                currentActivity_=Activity::Eating;
        }

    }else if(currentActivity_==Activity::Eating){

        auto temp=eatingActivity(legs,sensor,ma);
        bool targetFound=temp.first;
        bool eatenSomething=temp.second;

        if(!eatenSomething && !targetFound)
            // no food in seeing range
            // maybe there is some in Anthill
            currentActivity_=Activity::TakingFoodToAnthill;

        if(owner_->getEnergy()>=owner_->getMaxEnergy()-1)
            // fed
            currentActivity_=Activity::GoingToFoodSource;

    }else{
        std::cout<<"worker no activity"<<std::endl;
    }

    //if(legs->getTimeNotMoving()>0){
    //    if(biteAdjacentObstacle(sensor,ma))
    //        std::cout<<"bitten"<<std::endl;
    //}
    if(legs->getTimeNotMoving()>0){
        biteAdjacentObstacle(sensor,ma);
    }
    if(legs->getTimeNotMoving()>2 && currentActivity_!=Activity::Eating){
        panicTimeLeft_=4;
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
    if(sensor->getPheromoneStrength(PheromoneMap::Type::Anthill,owner_->getPos()) < 2){
        abd->dropAnthillPheromones();
    }
    
    if(owner_->getEnergy() < owner_->getMaxEnergy()*0.95){
        auto ret=eatingActivity(legs,sensor,ma);
        if(ret.first==false){
            // don't see any food
            legs->goRandom();
        }else{
            // sensed food but cannot acces it
            if(ret.second==false && legs->getTimeNotMoving()>0){
                biteAdjacentObstacle(sensor,ma);
            }
        }
    }

    if(owner_->getEnergy() > owner_->getMaxEnergy()*0.6){
        int foodCount=0;
        int antCount=0;
        for(auto o : sensor->getObservations()){
            if(o.getSmell()==Entity::Smell::Food)
                ++foodCount;
            if(o.getSmell()==Entity::Smell::Ant)
                ++antCount;
        }

        if(antCount<1 && foodCount>50){
            if(rand()%3)
                abd->bornWorker();
            else
                abd->bornScout();
        }else if(foodCount>25){
            if(rand()%10)
                abd->bornWorker();
            else
                abd->bornScout();
        }else if(foodCount<8){
            if(rand()%2)
                abd->bornScout();
            else
                abd->bornWorker();
        }
    }
}


AntScoutAI::AntScoutAI(Creature* owner):
    Controller(owner),
    panicTimeLeft_(0),
    currentActivity_(Activity::ScanningArea){
}

void AntScoutAI::step(int deltatime){
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


    if(currentActivity_==Activity::ScanningArea){
        // search food
        
        legs->goRandom();
            
        auto observations = sensor->getObservations();
        for(auto o : observations){
            if(sensor->getPheromoneStrength(PheromoneMap::Type::Anthill,o.getPos())>0.05)
                // this food is already in Anthill
                continue;

            if((o.getSmell()==Entity::Smell::Food) && !ma->isHolding()){
                if(o.getPos() != owner_->getPos()){ 
                    if(sensor->isAccessible(o)){
                        legs->goToPos(o.getPos());
                        break;
                    }
                }else{
                    ma->grab(o);
                    abd->dropToFoodPheromones();
                    currentActivity_=Activity::MarkingPathToFood;
                    break;
                }
            }
        }

        if(owner_->getEnergy()< 50)
            currentActivity_=Activity::Eating;
            // should return
            //currentActivity_=Activity::ReturnToAnthill;
        else
            // continue scanning activity
            abd->dropToFoodPheromones();

    }else if(currentActivity_==Activity::MarkingPathToFood ||
             currentActivity_==Activity::ReturnToAnthill){
        
        auto temp=returnToAnthill(legs,sensor,ma);
        bool returned=temp.first;
        bool lostTrace=temp.second;

        // don't mark path, if it is randomly chosen
        if(!lostTrace && currentActivity_==Activity::MarkingPathToFood)
            abd->dropFromFoodPheromones();

        if(returned){
            // check if should eat, or start scanning
            if(owner_->getEnergy()< 50)
                currentActivity_=Activity::Eating;
            else
                currentActivity_=Activity::ScanningArea;
        }else{
            if(owner_->getEnergy()< 50)
                currentActivity_=Activity::Eating;
        }

    }else if(currentActivity_==Activity::Eating){

        auto temp=eatingActivity(legs,sensor,ma);
        bool targetFound=temp.first;
        bool eatenSomething=temp.second;

        if(!eatenSomething && !targetFound)
            // no food
            //currentActivity_=Activity::ScanningArea;
            currentActivity_=Activity::ReturnToAnthill;

        if(owner_->getEnergy()>=owner_->getMaxEnergy()-1)
            // fed
            //currentActivity_=Activity::ScanningArea;
            currentActivity_=Activity::ReturnToAnthill;

    }else{
        std::cout<<"No activity"<<std::endl;
    }

    if(legs->getTimeNotMoving()>0){
        biteAdjacentObstacle(sensor,ma);
    }
    if(legs->getTimeNotMoving()>2 && currentActivity_!=Activity::Eating){
        panicTimeLeft_=2;
        legs->goRandom();
    }
}

CustomController::CustomController(Creature* owner,std::string filePath):
    Controller(owner){

    using namespace boost::python;

    // empty class for holding fun
    obj_=eval("lambda: None\n");
    
    // TODO: get code from external file
    // this is example
    exec(R"(
def step(owner):
    legs=owner.getAntLegs()
    legs[0].goRandom()
    )");

    object fun=eval("step");

    obj_.attr("step") = fun;
}

void CustomController::step(int deltaTime){
    obj_.attr("step")(owner_);
}
