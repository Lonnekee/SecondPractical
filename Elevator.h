#ifndef SECONDPRACTICAL_ELEVATOR_H
#define SECONDPRACTICAL_ELEVATOR_H

#include "Passenger.h"

using namespace std;

struct Elevator {
    static const int capacity = 2;
    static const int numberOfFloors = 5;
    int numberOfPassengers = 0;
    int currentFloor = 0;
    int goalFloors[capacity];
};


#endif //SECONDPRACTICAL_ELEVATOR_H
