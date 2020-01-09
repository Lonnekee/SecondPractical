#include <iostream>
#include "Elevator.h"
#include "Floor.h"
#include "State.h"
#include "LookupTable.h"
#include <math.h>
#include <float.h>

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
    int numberOfStates = pow(2, numberOfFloors) * pow((numberOfFloors * pow(2, numberOfFloors)), numberOfElevators);
    cout << "Number of states: " << numberOfStates << endl;

    int maxRepetitions = 10000;
    int maxEpochs = 10000;
    double alpha = 0.05;
    double discountFactor = 0.5;
    double epsilon = 0.1;
    LookupTable lookupTable;

    int optimalAction = rand() % 3; // First action
    double averageEpoch = 0.0;
    for (int repetitions = 1; repetitions <= maxRepetitions; repetitions++){
        State s(epsilon);

        int waiting = 0;
        Floor *floors = s.getFloors();
        for (int i = 0; i < numberOfFloors; i++) {
            waiting += floors[i].waitingPassengers.size();
        }
//        s.print();

        for (int epoch = 1; epoch <= maxEpochs; epoch++) {
            // Update state: each lift's action is performed (go up/down/stay),
            // randomly add waiting people on each floor.


            // int oldKey = get key s
            unsigned long long oldKey = lookupTable.fromStateToKey(s);
            int *actions = s.updateState(optimalAction);
            unsigned long long newKey = lookupTable.fromStateToKey(s);
            oldKey = lookupTable.addActionToKey(actions, oldKey);
            int reward = s.getReward();

            // Get the best action to take in this state
            double highestValue = -DBL_MIN;
            for (int i = 0; i < pow(3, numberOfElevators); i++) {
                int *possibleActions = new int[numberOfElevators]{i};
                unsigned long long key = lookupTable.addActionToKey(possibleActions, newKey);
                double value = lookupTable.getValue(key);
                if (value > highestValue) {
                    highestValue = value;
                    optimalAction = i;
                }
            }
            newKey *= 10;
            newKey += optimalAction;

            // Update lookupTable
            double newValue = (1 - alpha) * lookupTable.getValue(oldKey) +
                              alpha * (reward + discountFactor * lookupTable.getValue(newKey));
            lookupTable.setValue(oldKey, newValue);

            // End if there are no waiting people left
            if(s.areFloorsEmpty() || epoch == maxEpochs){
                if (waiting != 0) averageEpoch += epoch / waiting;
                break;
            }
        }

        if (repetitions % 100 == 0) {
            cout << "Average finishing epoch per waiting person: " << averageEpoch / 100 << endl;
            averageEpoch = 0.0;
        }
    }
    return 0;
}
