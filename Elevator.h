#ifndef SECONDPRACTICAL_ELEVATOR_H
#define SECONDPRACTICAL_ELEVATOR_H

#include "Passenger.h"
#include "Constants.h"

using namespace std;

struct Elevator {
    int currentFloor = rand() % numberOfFloors;
    int goalFloors[numberOfFloors] = {0};
    vector<Passenger> passengers;
};


#endif //SECOND PRACTICAL_ELEVATOR_H
