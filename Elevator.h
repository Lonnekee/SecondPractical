#ifndef SECONDPRACTICAL_ELEVATOR_H
#define SECONDPRACTICAL_ELEVATOR_H

#include "Passenger.h"

using namespace std;

struct Elevator {
    static const int capacity = 4;
    static const int numberOfFloors = 5;
    int currentFloor = rand() % numberOfFloors;
    int goalFloors[numberOfFloors] = {0};
    vector<Passenger> passengers;
};


#endif //SECOND PRACTICAL_ELEVATOR_H
