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

    void updateFloors() {
        for(int i = 0; i < numberOfFloors; i++) {
            int maximum = floors[i].maximumWaiting;
            int length = floors[i].waitingPassengers.size();

            if (length < maximum) {
                default_random_engine generator;
                poisson_distribution<int> distribution(0.5);

                numberOfNewPeople = distribution(generator);
                for (int j = 0; j < numberOfNewPeople && j < maximum - length; j++) {
                    Passenger p;
                    floors[i].waitingPassengers.push_back(p);
                }
            }
        }
    }

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
    void selectActions();
    void updateState();

    // Getters and setters
    Floor *getFloors() { return floors; }
    Elevator *getElevators() { return elevators; }
};


#endif //SECONDPRACTICAL_STATE_H
