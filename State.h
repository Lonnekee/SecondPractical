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

    void performAction(int numberOfElevator){
        int randomNumber = rand() % 4;
        Passenger p;
        Elevator e = elevators[numberOfElevator];
        switch(randomNumber){
            case 1 : // Elevator goes up
                if(elevators[numberOfElevator].currentFloor != 4){
                    elevators[numberOfElevator].currentFloor++;
                    break;
                }
            case 2 : // Elevator goes down
                if(elevators[numberOfElevator].currentFloor != 0){
                    elevators[numberOfElevator].currentFloor--;
                    break;
                }
            case 3 :
                if(elevators[numberOfElevator].numberOfPassengers > 0){
                    for(int i = 0; i<elevators[numberOfElevator].capacity; i++){
                        if(elevators[numberOfElevator].goalFloors[0] == elevators[numberOfElevator].currentFloor){
                            elevators[numberOfElevator].goalFloors[0] = -1;
                            elevators[numberOfElevator].numberOfPassengers--;
                            // Add reward
                        }
                    }
                }
                int addWaiting = elevators[numberOfElevator].capacity - elevators[numberOfElevator].numberOfPassengers;
                int addFloor = 0;
                for(int i = 0; i<addWaiting; i++){
                    addFloor = floors[elevators[numberOfElevator].currentFloor].waitingPassengers[0].goalFloor;
                    for(int j = 0; j < elevators[numberOfElevator].capacity; j++){
                        if(elevators[numberOfElevator].goalFloors[j] == -1){
                            elevators[numberOfElevator].goalFloors[j] = addFloor;
                            floors[elevators[numberOfElevator].currentFloor].waitingPassengers.pop_back();
                            break;
                        }
                    }
                }
        }
    }
    void updateState();

    // Getters and setters
    Floor *getFloors() { return floors; }
    Elevator *getElevators() { return elevators; }
};


#endif //SECONDPRACTICAL_STATE_H
