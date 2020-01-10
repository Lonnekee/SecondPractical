#ifndef SECONDPRACTICAL_UTIL_H
#define SECONDPRACTICAL_UTIL_H

#include <fstream>
#include "LookupTable.h"
#include "float.h"

int seed = 0; // TODO
int getSeed() { return seed++; }

int getBestAction(LookupTable lookupTable, unsigned long long newKey) {
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
    return optimalAction;
}

int eGreedyActionSelection(double epsilon, int bestAction) {
    default_random_engine generator(getSeed());
    uniform_real_distribution<double> dist(0.0, 1.0);
    double ranNum = dist(generator);

    if (ranNum > epsilon) { // Choose the greedy action
        return bestAction;
    } else { // Choose a random action
        return rand() % 3;
    }
}

void printToFile(string filename, int epochs, double *results) {
    cout << "Writing to file..." << endl;
    ofstream myfile ("results/" + filename + ".csv");
    if (myfile.is_open())
    {
        myfile << "epoch, result\n";
        for(int index = 0; index < epochs; index ++){
            myfile << index+1 << "," << results[index] << "\n";
        }
        myfile.close();
        cout << "Done." << endl;
    }
    else cout << "Unable to open file";
}

#endif //SECONDPRACTICAL_UTIL_H
