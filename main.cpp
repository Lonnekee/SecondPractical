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
    int maximumWaiting = 4, numberOfFloors = 5, capacity = 4, numberOfLifts = 1;
    int numberOfStates = pow(2, numberOfFloors) * pow((numberOfFloors * pow(2, numberOfFloors)), numberOfLifts);
    cout << "Number of states: " << numberOfStates << endl;

    int maxRepetitions = 100000;
    int maxEpochs = 10000;
    double alpha = 0.05;
    double discountFactor = 0.5;
    double epsilon = 0.1;
    LookupTable lookupTable;

    int optimalAction = rand() % 3; // First action
    int averageEpoch = 0;
    for (int repetitions = 1; repetitions <= maxRepetitions; repetitions++){
        State s(epsilon);
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
            for (int i = 0; i < pow(3, numberOfLifts); i++) {
                int *possibleActions = new int[numberOfLifts]{i};
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

            // Print the new state
//            s.print();
            //cout << "Reward: " << reward << endl;
//            cout << "Average reward: " << s.totalReward / epoch << endl;
            if(s.areFloorsEmpty() || epoch == maxEpochs  ){
//                cout << "Epoch: " << epoch << endl;
                averageEpoch += epoch;
                break;
            }
        }

        if (repetitions % 100 == 0) {
            cout << "Average finishing epoch: " << averageEpoch / 100 << endl;
            averageEpoch = 0;

            //lookupTable.print();
        }
    }
    return 0;
}

void getValue(int i) {

}
