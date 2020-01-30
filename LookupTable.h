#ifndef SECONDPRACTICAL_LOOKUPTABLE_H
#define SECONDPRACTICAL_LOOKUPTABLE_H

#include <map>
#include <cfloat>

using namespace std;

class LookupTable {
private:
    map<unsigned int, double> lookupTable;

    void addEntry(unsigned int key) {
        int value = rand() % 11 - 5;
        lookupTable.insert(make_pair(key, value));
    }
public:
    int getOptimalAction(unsigned int key) {
        int optimalAction = 0;
        double highestValue = -DBL_MIN;

        for (int i = 0; i < pow(3, numberOfElevators); i++) {
            int *possibleActions = new int[numberOfElevators]{i};
            unsigned int actionKey = addActionToKey(possibleActions[0], key);
            double value = getValue(actionKey);
            if (value > highestValue) {
                highestValue = value;
                optimalAction = i;
            }
        }

        return optimalAction;
    }

    unsigned int addActionToKey(int action, unsigned int key) {
        for (int i = 0; i < numberOfElevators; i++) {
            key *= 10;
            key += action;
        }
        return key;
    }

    double getValue(unsigned int key) {
        auto it = lookupTable.find(key);
        if (it != lookupTable.end()) {
            return it->second;
        } else {
            addEntry(key);
            return 0;
        }
    }

    void setValue(unsigned int key, double value) {
        auto it = lookupTable.find(key);
        if (it != lookupTable.end()) {
            it->second = value;
        }
    }

    int printAmountOfEntries() {
        int amount = 0;
        for(auto it = lookupTable.cbegin(); it != lookupTable.cend(); ++it) {
            //cout << it->first << " " << it->second << endl;
            amount++;
        }
        return amount;
    }
};

#endif //SECONDPRACTICAL_LOOKUPTABLE_H
