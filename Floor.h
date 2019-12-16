#ifndef SECONDPRACTICAL_FLOOR_H
#define SECONDPRACTICAL_FLOOR_H

#include "Passenger.h"

struct Floor {
    int maximumWaiting = 3;
    Passenger *waitingPassengers = new Passenger[maximumWaiting];
};

#endif //SECONDPRACTICAL_FLOOR_H
