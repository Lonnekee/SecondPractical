#ifndef SECONDPRACTICAL_LOOKUPTABLE_H
#define SECONDPRACTICAL_LOOKUPTABLE_H

#include <map>

using namespace std;

class LookupTable {
private:
    map<unsigned long long, double> lookupTable;

    int bitsToInt(bool *bits, int length) {
        int number = 0;
        for (int i = 0; i < length; i++) {
            number += pow(2,i) * bits[i];
        }
        return number;
    }

    void addEntry(unsigned long long key) {
        int value = rand() % 11 - 5;
        lookupTable.insert(make_pair(key, value));
    }
public:
    /**
     * Converts a state to a key.
     *
     * @pre The number of floors is max 9.
     * @param floors
     * @param elevators
     * @return
     */
    unsigned long long fromStateToKey(Floor *floors, Elevator *elevators) {
            unsigned long long key = 1000;

            // Iterate over all floors
            bool bits[numberOfFloors];
            for (int i = 0; i < numberOfFloors; i++) {
                bits[i] = (floors[i].waitingPassengers.size() != 0) ? 1 : 0;
            }
            key += bitsToInt(bits,numberOfFloors);
            key *= 10;

        // Iterate over all elevators
        for (int i = 0; i < numberOfElevators; i++) {
            key += elevators[i].currentFloor;
            key *= 1000;

            for (int j = 0; j < numberOfFloors; j++) {
                bits[j] = elevators[i].goalFloors[j];
            }
            key += bitsToInt(bits,numberOfFloors);
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
