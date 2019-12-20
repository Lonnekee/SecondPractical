#include <iostream>
#include "Elevator.h"
#include "Floor.h"
#include "State.h"
#include <math.h>

using namespace std;

int main() {
    /*
     * number of states = (2 ^ numberOfFloors) * ( (numberOfFloors * 2 ^ numberOfFloors) ^ numberOfLifts)
     *
     * Valid options:
     * - 648000: int maximumWaiting = 4, numberOfFloors = 3, capacity = 2, numberOfLifts = 2;
     * - 819199: int maximumWaiting = 3, numberOfFloors = 5, capacity = 4, numberOfLifts = 1;
     */

//      To calculate the number of states:
    int maximumWaiting = 4, numberOfFloors = 3, capacity = 2, numberOfLifts = 2;
    int numberOfStates = pow(2,numberOfFloors) * pow((numberOfFloors * pow(2, numberOfFloors)), numberOfLifts);
    cout << numberOfStates << endl;
    int maxEpochs = 100;
    State s;
    s.print();

    for (int epoch = 1; epoch < maxEpochs; epoch++) {
        // Update state: each lift's action is performed (go up/down/stay),
        // randomly add waiting people on each floor.
        s.updateState();
        s.print();
        cout << "Average reward: " << s.totalReward / epoch << endl;
    }

    return 0;
}
