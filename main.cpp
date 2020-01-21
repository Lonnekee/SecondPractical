#include <iostream>
#include "Elevator.h"
#include "Floor.h"
#include "State.h"
#include "LookupTable.h"
#include "Util.h"
#include <math.h>
#include <iostream>
#include <chrono>
#include <ctime>


using namespace std;

int main() {
    /*
     * number of states = (2 ^ numberOfFloors) * ( (numberOfFloors * 2 ^ numberOfFloors) ^ numberOfLifts)
     *
     * Valid options:
     * - 5120: int maximumWaiting = 4, numberOfFloors = 5, capacity = 4, numberOfLifts = 1;
     * - 819200: int maximumWaiting = 4, numberOfFloors = 5, capacity = 4, numberOfLifts = 2;
     */

    // To calculate the number of states:
    int numberOfStates = pow(2, numberOfFloors) * pow((numberOfFloors * pow(2, numberOfFloors)), numberOfElevators);
    cout << "Number of states: " << numberOfStates << endl;
    double epsilon = 0;

    ofstream myfile("Parameter Sweep/SarsaEpsilonSecond");
    myfile << "Number of floors: " << numberOfFloors << " maximum waiting: " << maximumWaiting << endl;
    myfile << "(optimized) Alpha: " << alpha << endl;
    myfile << "Gamma: " << gamma << endl;

    for(epsilon = 0.225; epsilon < 0.25; epsilon += 0.005) {
        double totalValue = 0;
        int algorithm = 2; //1 for q learning, 2 for sarsa
        int maxRepetitions = 100000;
        int maxEpochs = 10000;
        LookupTable lookupTable;
        double results[maxRepetitions / 100];

        double averageEpoch = 0.0;

        auto start = std::chrono::system_clock::now();

        if (algorithm == 1) {
            for (int repetitions = 1; repetitions <= maxRepetitions; repetitions++) {
                State s(epsilon);
                int action = rand() % 3;
                int waiting = 0;
                Floor *floors = s.getFloors();
                for (int i = 0; i < numberOfFloors; i++) {
                    waiting += floors[i].waitingPassengers.size();
                }


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
                    if ((s.areFloorsEmpty() && s.getElevators()->passengers.empty()) || epoch == maxEpochs) {
                        double newValue = (1 - alpha) * lookupTable.getValue(oldKey) +
                                          alpha * (1 + gamma * lookupTable.getValue(newKey));
                        lookupTable.setValue(oldKey, newValue);
                        if (waiting != 0) averageEpoch += epoch / waiting;
                        break;
                    }
                    // Update lookupTable
                    double newValue = (1 - alpha) * lookupTable.getValue(oldKey) +
                                      alpha * (reward + gamma * lookupTable.getValue(newKey));
                    lookupTable.setValue(oldKey, newValue);


                }

                // Print results
                if (repetitions % 100 == 0) {
                    results[repetitions / 100 - 1] = averageEpoch / 100;
                    //cout << "Average amount of epochs in which a person reaches their floor: " << averageEpoch / 100 << endl;
                    averageEpoch = 0.0;
                }
                if (repetitions > 10000) {
                    auto end = std::chrono::system_clock::now();
                    std::chrono::duration<double> elapsed_seconds = end - start;
                    if (elapsed_seconds.count() > 180.00) {
                        totalValue = -1;
                        break;
                    }
                }
            }
        } else {
            for (int repetitions = 1; repetitions <= maxRepetitions; repetitions++) {
                rand();
                State s(epsilon);
                int action = rand() % 2 + 1;
                int actionNew = action;

                int waiting = 0;
                Floor *floors = s.getFloors();
                for (int i = 0; i < numberOfFloors; i++) {
                    waiting += floors[i].waitingPassengers.size();
                }

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
                    unsigned long long newKey = lookupTable.fromStateToKey(s.getFloors(), s.getElevators());

                    int reward = 0;

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

                    // Update lookupTable with a reward of 0 because it is not the final state
                    double newValue = (1-alpha) * lookupTable.getValue(oldKey) +
                                      alpha * (0 + (gamma * (lookupTable.getValue(newKey))));
                    lookupTable.setValue(oldKey, newValue);

                    action = actionNew;
                    oldKey = newKey;

                    // End if there are no waiting people left
                    if ((s.areFloorsEmpty() && s.getElevators()->passengers.empty()) || epoch == maxEpochs) {
                        //If it is the final state, give a reward of 1 and update the lookup table
                        double newValue = (1-alpha) * lookupTable.getValue(oldKey) +
                                          alpha * (1 + (gamma * (lookupTable.getValue(newKey))));
                        lookupTable.setValue(oldKey, newValue);
                        if (waiting != 0) averageEpoch += epoch / waiting;
                        break;
                    }
                }

                //Print results
                if (repetitions % 100 == 0) {
                    results[repetitions / 100 - 1] = averageEpoch / 100;
                    //cout << "Average finishing epoch per waiting person: " << averageEpoch / 100 << endl;
                    averageEpoch = 0.0;
                }

                if (repetitions > 10000) {
                    auto end = std::chrono::system_clock::now();
                    std::chrono::duration<double> elapsed_seconds = end - start;
                    if (elapsed_seconds.count() > 180.00) {
                        totalValue = -1;
                        break;
                    }
                }
            }
        }
        if(totalValue == -1){
            myfile << "With epsilon = " << epsilon << ", the program took more than 3 minutes and was terminated." << endl;
        }
            totalValue = 0;
            for (int i = (maxRepetitions / 100) - 100; i < (maxRepetitions / 100); i++) {
                totalValue += results[i];
            }
            cout << "Epsilon: " << epsilon << " with the average of last 100 reps: " << totalValue / 100 << endl;
            auto end = std::chrono::system_clock::now();
            std::chrono::duration<double> elapsed_seconds = end - start;
            std::time_t end_time = std::chrono::system_clock::to_time_t(end);

            if (myfile.is_open()) {
                myfile << "With epsilon = " << epsilon << ", performance = " << totalValue / 100 << " and elapsed time: " << elapsed_seconds.count() << endl;
            } else cout << "Unable to open file";

    }
    // cout << "Entries in lookup table: " << lookupTable.printAmountOfEntries() << endl;
    //printToFile("Sarsa9-5high", maxEpochs, results);
    myfile.close();
    return 0;
}
