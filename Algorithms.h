#ifndef SECONDPRACTICAL_ALGORITHMS_H
#define SECONDPRACTICAL_ALGORITHMS_H


#include "State.h"
#include "Constants.h"

class Algorithms {
private:
    int waiting = 0;
    int action = -1;
    double averageEpoch = 0.0;
    int n = 0;
    State s;

    void initialise() {
        s = State();
        action = rand() % 3;
        waiting = 0;
        Floor *floors = s.getFloors();
        for (int i = 0; i < numberOfFloors; i++) {
            waiting += floors[i].waitingPassengers.size();
        }
    }

    void addToAverageEpoch(double epochPerWaiting) {
        averageEpoch += (epochPerWaiting - averageEpoch)/(double) ++n;
    }

    void resetAverageEpoch() {
        averageEpoch = 0.0;
        n = 0;
    }

    bool hasGoalsAbove(Elevator e) {
        Floor *floors = s.getFloors();
        for (int floor = e.currentFloor+1; floor < numberOfFloors; floor++) {
            if (e.goalFloors[floor] || floors[floor].waitingPassengers.size()) {
                // There is a floor above that the elevator should go to
                return true;
            }
        }
        return false;
    }

    bool hasGoalsBelow(Elevator e) {
        Floor *floors = s.getFloors();
        for (int floor = e.currentFloor-1; floor >= 0; floor--) {
            if (e.goalFloors[floor] || floors[floor].waitingPassengers.size()) {
                // There is a floor below that the elevator should go to
                return true;
            }
        }
        return false;
    }

public:
    void runQlearning(int maxRepetitions, int maxEpochs, LookupTable lookupTable) {
        std::cout << "Running Q-learning..." << endl;

        for (int repetition = 1; repetition <= maxRepetitions; repetition++) {
            initialise();
            for (int epoch = 1; epoch <= maxEpochs; epoch++) {
                unsigned int oldKey = s.toKey();
                s.updateState(action); // Take the action
                unsigned int newKey = s.toKey();

                oldKey = lookupTable.addActionToKey(action, oldKey);

                // Get the best action to take in this state
                int optimalAction = lookupTable.getOptimalAction(newKey);
                newKey *= 10;
                newKey += optimalAction;

                action = eGreedyActionSelection(epsilon, optimalAction);

                // Update lookupTable
                int reward = s.getReward();
                double newValue = (1 - alpha) * lookupTable.getValue(oldKey) +
                                  alpha * (reward + gamma * lookupTable.getValue(newKey));
                lookupTable.setValue(oldKey, newValue);

                // End if there are no waiting people left
                if (s.isTerminal() || epoch == maxEpochs) {
                    if (waiting != 0) addToAverageEpoch(epoch/(double)waiting);
                    break;
                }
            }

            if (repetition % 100 == 0) {
                cout << ((double) repetition)/maxRepetitions*100 << "%: ";
                cout << averageEpoch << endl;
                resetAverageEpoch();
            }
        }
    }

    void runSarsa(int maxRepetitions, int maxEpochs, LookupTable lookupTable) {
        std::cout << "Running SARSA..." << endl;

        for (int repetition = 1; repetition <= maxRepetitions; repetition++) {
            initialise();
            unsigned int oldKey = s.toKey();
            int optimalAction = lookupTable.getOptimalAction(oldKey);
            action = eGreedyActionSelection(epsilon, optimalAction);
            oldKey *= 10;
            oldKey += action;

            // Main loop
            for (int epoch = 1; epoch <= maxEpochs; epoch++) {
                s.updateState(action);
                unsigned int newKey = s.toKey();

                // Get the best action to take in this state
                optimalAction = lookupTable.getOptimalAction(newKey);
                int actionNew = eGreedyActionSelection(epsilon, optimalAction);
                newKey *= 10;
                newKey += actionNew;

                // Update lookupTable
                int reward = s.getReward();
                double newValue = (1 - alpha) * lookupTable.getValue(oldKey) +
                                  alpha * (reward + gamma * lookupTable.getValue(newKey));
                lookupTable.setValue(oldKey, newValue);

                action = actionNew;
                oldKey = newKey;

                // End if there are no waiting people left
                if (s.isTerminal() || epoch == maxEpochs) {
                    if (waiting != 0) addToAverageEpoch(epoch/(double)waiting);
                    break;
                }
            }

            if (repetition % 100 == 0) {
                cout << ((double) repetition)/maxRepetitions*100 << "%: ";
                cout << averageEpoch << endl;
                resetAverageEpoch();
            }
        }
    }

    void runNormal(int maxRepetitions, int maxEpochs) {
        cout << "Running normal elevator..." << endl;

        for (int repetition = 1; repetition <= maxRepetitions; repetition++) {
            initialise();
            bool goingUp[numberOfElevators] = {true};
            bool stayedAtCurrentFloor[numberOfElevators] = {false};

            for (int epoch = 1; epoch <= maxEpochs; epoch++) {
                Elevator *elevators = s.getElevators();
                for(int i = 0; i < numberOfElevators; i++) {
                    Elevator e = elevators[i];
                    if (e.goalFloors[e.currentFloor] == 1 // passengers want to get off here
                        || (s.getFloors()[e.currentFloor].waitingPassengers.size() // there are people waiting here to get on
                        && !stayedAtCurrentFloor[i] ) // and we haven't yet let them enter. (If we have, the elevator is full.)
                        ) {
                        action = STAY;
                        stayedAtCurrentFloor[i] = true;
                    } else {
                        stayedAtCurrentFloor[i] = false;

                        if (goingUp[i] && hasGoalsAbove(e)) {
                            // Was going up and should continue
                            action = UP;
                        } else if (!goingUp[i] && hasGoalsBelow(e)) {
                            // Was going down and should continue
                            action = DOWN;
                        } else if (hasGoalsAbove(e)) {
                            // Was not going up, but should now go up.
                            action = UP;
                            goingUp[i] = true;
                        } else if (hasGoalsBelow(e)) {
                            // Was not going down, but should now go down.
                            action = DOWN;
                            goingUp[i] = false;
                        } else {
                            // There are no goals, so stay.
                            action = STAY;
                            stayedAtCurrentFloor[i] = true;
                        }
                    }
                }

                s.updateState(action);

                // End if there are no waiting people left
                if (s.isTerminal() || epoch == maxEpochs) {
                    if (waiting != 0) addToAverageEpoch(epoch/(double)waiting);
                    break;
                }
            }

            if (repetition % 100 == 0) {
                cout << ((double) repetition)/maxRepetitions*100 << "%: ";
                cout << averageEpoch << endl;
                resetAverageEpoch();
            }
        }
    }
};


#endif //SECONDPRACTICAL_ALGORITHMS_H
