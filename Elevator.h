#ifndef SECONDPRACTICAL_ELEVATOR_H
#define SECONDPRACTICAL_ELEVATOR_H

#include "Passenger.h"

using namespace std;

struct Elevator {
    static const int capacity = 4;
    static const int numberOfFloors = 5;
    int numberOfPassengers = 0;
    int currentFloor = 0;
    vector<int> goalFloors;
};


#endif //SECONDPRACTICAL_ELEVATOR_H
