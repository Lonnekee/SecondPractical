//
// Created by lonne on 16-12-2019.
//

#ifndef SECONDPRACTICAL_STATE_H
#define SECONDPRACTICAL_STATE_H


#include "Elevator.h"
#include "Floor.h"

class State {
private:
    Elevator *elevators;
    Floor *floors;
public:
    State(int numberOfElevators, int numberOfFloors) {
        elevators = new Elevator[numberOfElevators];
        floors = new Floor[numberOfFloors];
    }
    void giveRewards();
    void updateState();
};


#endif //SECONDPRACTICAL_STATE_H
