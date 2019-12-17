#ifndef SECONDPRACTICAL_ELEVATOR_H
#define SECONDPRACTICAL_ELEVATOR_H

#include "Passenger.h"

struct Elevator {
    int capacity = 4;
    int numberOfFloors = 5;
    int numberOfPassengers = 0;
    int currentFloor = 0;
    int *goalFloors = new int[numberOfFloors];
};


#endif //SECONDPRACTICAL_ELEVATOR_H
