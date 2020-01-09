#ifndef SECONDPRACTICAL_LOOKUPTABLE_H
#define SECONDPRACTICAL_LOOKUPTABLE_H

#include <map>

using namespace std;

class LookupTable {
private:
    map<unsigned long long, double> lookupTable;

    void addEntry(unsigned long long key) {
        int value = rand() % 11 - 5;
        lookupTable.insert(make_pair(key, value));
    }
public:
    unsigned long long fromStateToKey(Floor *floors, Elevator *elevators) {
            unsigned long long key = 0;

            // Iterate over all floors
            for (int i = 0; i < numberOfFloors; i++) {
                key += (floors[i].waitingPassengers.size() != 0) ? 2 : 1;
                key *= 10;
            }

        // Iterate over all elevators
        for (int i = 0; i < numberOfElevators; i++) {
            key += elevators[i].currentFloor;

            for (int j = 0; j < numberOfFloors; j++) {
                key *= 10;
                key += elevators[i].goalFloors[j];
            }
        }
        return key;
    }

    unsigned long long addActionToKey(int action, unsigned long long key) {
        for (int i = 0; i < numberOfElevators; i++) {
            key *= 10;
            key += action;
        }
        return key;
    }

    double getValue(unsigned long long key) {
        auto it = lookupTable.find(key);
        if (it != lookupTable.end()) {
            return it->second;
        } else {
            addEntry(key);
            return 0;
        }
    }

    void setValue(unsigned long long key, double value) {
        auto it = lookupTable.find(key);
        if (it != lookupTable.end()) {
            it->second = value;
        }
    }

    void print() {
        for(auto it = lookupTable.cbegin(); it != lookupTable.cend(); ++it)
        {
            cout << it->first << " " << it->second << endl;
        }
    }
};

#endif //SECONDPRACTICAL_LOOKUPTABLE_H
