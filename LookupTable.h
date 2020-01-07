#ifndef SECONDPRACTICAL_LOOKUPTABLE_H
#define SECONDPRACTICAL_LOOKUPTABLE_H

#include <map>
#include "State.h"
#include <typeinfo>

using namespace std;

class LookupTable {
private:
    static const int numberOfFloors = 5;
    static const int numberOfElevators = 1;
    map<unsigned long long, double> lookupTable;

    void addEntry(unsigned long long key) {
        int value = rand() % 11 - 5;
        lookupTable.insert(make_pair(key, value));
    }
public:
    unsigned long long fromStateToKey(State s) {
        unsigned long long key = 0;

        // Iterate over all floors
        Floor *floors = s.getFloors();
        for (int i = 0; i < numberOfFloors; i++) {
            key += i + 1;
            key *= 10;
            key += (floors[i].waitingPassengers.size() != 0) ? 1 : 0;
            key *= 10;
        }

        // Iterate over all elevators
        Elevator *elevators = s.getElevators();
        for (int i = 0; i < numberOfElevators; i++) {
            key += i + 1;
            key *= 10;
            key += elevators[i].currentFloor;

            for (int j = 0; j < numberOfFloors; j++) {
                key *= 10;
                key += elevators[i].goalFloors[j];
            }
        }
        return key;
    }

    int addActionToKey(int *action, unsigned long long key) {
        for (int i = 0; i < numberOfElevators; i++) {
            key *= 10;
            key += action[i];
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
