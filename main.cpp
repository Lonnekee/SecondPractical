#include <iostream>
#include "Elevator.h"
#include <math.h>

using namespace std;

int main() {
    /*
     * number of states = (maximumWaiting+1 ^ numberOfFloors) * ( ( (capacity+1) * numberOfFloors * 2 ^ numberOfFloors) ^ numberOfLifts)
     *
     * Valid options:
     * - maximumWaiting: 4, numberOfFloors: 3, capacity: 2, numberOfLifts: 2
     * - maximumWaiting: 3, numberOfFloors: 5, capacity: 4, numberOfLifts: 1
     */

    int maximumWaiting = 3, numberOfFloors = 3, capacity = 4, numberOfLifts = 1;
    int numberOfStates = pow(maximumWaiting+1,numberOfFloors) * pow( pow( pow((capacity+1), numberOfFloors), pow(2, numberOfFloors)), numberOfLifts);
    cout << numberOfStates << endl;

    return 0;
}