#include <iostream>
#include "Elevator.h"
#include "Floor.h"
#include "State.h"
#include "LookupTable.h"
#include "Util.h"
#include <math.h>

using namespace std;

int main() {
    /*
     * number of states = (2 ^ numberOfFloors) * ( (numberOfFloors * 2 ^ numberOfFloors) ^ numberOfLifts)
     *
     * Valid options:
     * - 5120: int maximumWaiting = 4, numberOfFloors = 5, capacity = 4, numberOfLifts = 1;
     * - 819200: int maximumWaiting = 4, numberOfFloors = 5, capacity = 4, numberOfLifts = 2;
     */

//      To calculate the number of states:
    int numberOfStates = pow(2, numberOfFloors) * pow((numberOfFloors * pow(2, numberOfFloors)), numberOfElevators);
    cout << "Number of states: " << numberOfStates << endl;
    int algorithm = 2; //1 for q learning, 2 for sarsa
    int maxRepetitions = 100000;
    int maxEpochs = 10000;
    double alpha = 0.05;
    double discountFactor = 0.5;
    double epsilon = 0.1;
    LookupTable lookupTable;

    double averageEpoch = 0.0;

    if(algorithm == 1) {
        for (int repetitions = 1; repetitions <= maxRepetitions; repetitions++) {
            State s(epsilon);
            int action = rand() % 3;
            int waiting = 0;
            Floor *floors = s.getFloors();
            for (int i = 0; i < numberOfFloors; i++) {
                waiting += floors[i].waitingPassengers.size();
            }
//        s.print();

            // Main loop
            for (int epoch = 1; epoch <= maxEpochs; epoch++) {
                unsigned long long oldKey = lookupTable.fromStateToKey(s.getFloors(), s.getElevators());
                s.updateState(action);

                unsigned long long newKey = lookupTable.fromStateToKey(s.getFloors(), s.getElevators());

                oldKey = lookupTable.addActionToKey(action, oldKey);
                int reward = 0;

                // Get the best action to take in this state
                int optimalAction = 0;
                double highestValue = -DBL_MIN;
                for (int i = 0; i < pow(3, numberOfElevators); i++) {
                    int *possibleActions = new int[numberOfElevators]{i};
                    unsigned long long key = lookupTable.addActionToKey(possibleActions[0], newKey);
                    double value = lookupTable.getValue(key);
                    if (value > highestValue) {
                        highestValue = value;
                        optimalAction = i;
                    }
                }
                newKey *= 10;
                newKey += optimalAction;

                action = eGreedyActionSelection(epsilon, optimalAction);

                // End if there are no waiting people left
                if (s.areFloorsEmpty() || epoch == maxEpochs) {
                    double newValue = (1 - alpha) * lookupTable.getValue(oldKey) +
                                      alpha * (1 + discountFactor * lookupTable.getValue(newKey));
                    lookupTable.setValue(oldKey, newValue);
                    if (waiting != 0) averageEpoch += epoch / waiting;
                    break;
                }
                // Update lookupTable
                double newValue = (1 - alpha) * lookupTable.getValue(oldKey) +
                                  alpha * (reward + discountFactor * lookupTable.getValue(newKey));
                lookupTable.setValue(oldKey, newValue);


            }

            // Print results
            if (repetitions % 100 == 0) {
                cout << "Average finishing epoch per waiting person: " << averageEpoch / 100 << endl;
                averageEpoch = 0.0;
            }
        }
    } else {
        for (int repetitions = 1; repetitions <= maxRepetitions; repetitions++) {
            State s(epsilon);
            int action = rand() % 3;
            int actionNew = action;

            int waiting = 0;
            Floor *floors = s.getFloors();
            for (int i = 0; i < numberOfFloors; i++) {
                waiting += floors[i].waitingPassengers.size();
            }

//        s.print();

            unsigned long long oldKey = lookupTable.fromStateToKey(s.getFloors(), s.getElevators());
            // Get the best action to take in this state
            int optimalAction = 0;
            double highestValue = -DBL_MIN;
            for (int i = 0; i < pow(3, numberOfElevators); i++) {
                int *possibleActions = new int[numberOfElevators]{i};
                unsigned long long key = lookupTable.addActionToKey(possibleActions[0], oldKey);
                double value = lookupTable.getValue(key);
                if (value > highestValue) {
                    highestValue = value;
                    optimalAction = i;
                }
            }
            action = eGreedyActionSelection(epsilon, optimalAction);
            oldKey *= 10;
            oldKey += action;

            // Main loop
            for (int epoch = 1; epoch <= maxEpochs; epoch++) {
                s.updateState(action);
                //int reward = s.getReward();
                unsigned long long newKey = lookupTable.fromStateToKey(s.getFloors(), s.getElevators());

                // Get the best action to take in this state
                optimalAction = 0;
                double highestValue = -DBL_MIN;
                for (int i = 0; i < pow(3, numberOfElevators); i++) {
                    int *possibleActions = new int[numberOfElevators]{i};
                    unsigned long long key = lookupTable.addActionToKey(possibleActions[0], newKey);
                    double value = lookupTable.getValue(key);
                    if (value > highestValue) {
                        highestValue = value;
                        optimalAction = i;
                    }
                }
                actionNew = eGreedyActionSelection(epsilon, optimalAction);
                newKey *= 10;
                newKey += actionNew;

                // Update lookupTable
                double newValue = (1 - alpha) * lookupTable.getValue(oldKey) +
                                  alpha * (0 + (discountFactor * (lookupTable.getValue(newKey))) - lookupTable.getValue(oldKey));
                lookupTable.setValue(oldKey, newValue);

                action = actionNew;
                oldKey = newKey;

                // End if there are no waiting people left
                if (s.areFloorsEmpty() || epoch == maxEpochs) {
                    double newValue = (1 - alpha) * lookupTable.getValue(oldKey) +
                                      alpha * (1 + (discountFactor * (lookupTable.getValue(newKey))) - lookupTable.getValue(oldKey));
                    lookupTable.setValue(oldKey, newValue);
                    if (waiting != 0) averageEpoch += epoch / waiting;
                    break;
                }
            }

            // Print results
            if (repetitions % 100 == 0) {
                cout << "Average finishing epoch per waiting person: " << averageEpoch / 100 << endl;
                averageEpoch = 0.0;
            }
        }
    }
    return 0;
}
