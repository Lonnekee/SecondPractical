#ifndef SECONDPRACTICAL_STATE_H
#define SECONDPRACTICAL_STATE_H


#include "Elevator.h"
#include "Floor.h"

using namespace std;

class State {
private:
    static const int numberOfFloors = 5;
    static const int numberOfElevators = 1;

    Elevator elevators[numberOfElevators];
    Floor floors[numberOfFloors];

public:
    State() {
        for (int i = 0; i < numberOfFloors; i++) {
            int numberOfWaiters = rand() % 4;
            for (int j = 0; j < numberOfWaiters; j++) {
                Passenger p;
                floors[i].waitingPassengers.push_back(p);
            }
        }
    }
    void giveRewards();
    void updateState();

    // Getters and setters
    Floor *getFloors() { return floors; }
    Elevator *getElevators() { return elevators; }
};


#endif //SECONDPRACTICAL_STATE_H
