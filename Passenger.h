#ifndef SECONDPRACTICAL_PASSENGER_H
#define SECONDPRACTICAL_PASSENGER_H

#include <random>
#include "Constants.h"

struct Passenger {
    int goalFloor = rand() % numberOfFloors;
};

#endif //SECONDPRACTICAL_PASSENGER_H
