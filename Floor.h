#ifndef SECONDPRACTICAL_FLOOR_H
#define SECONDPRACTICAL_FLOOR_H

#include "Passenger.h"

using namespace std;

class Floor {
public:
    static const int maximumWaiting = 4;
    vector<Passenger> waitingPassengers;
};

#endif //SECONDPRACTICAL_FLOOR_H
