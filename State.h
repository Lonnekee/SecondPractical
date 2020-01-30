#ifndef SECONDPRACTICAL_STATE_H
#define SECONDPRACTICAL_STATE_H

#include "Elevator.h"
#include "Floor.h"
#include "Util.h"
#include "Constants.h"
#include <float.h>
#include <iostream>

using namespace std;

static std::random_device rd;
static std::mt19937 gen(rd());

class State {
private:
    Elevator elevators[numberOfElevators];
    Floor floors[numberOfFloors];
    int actions[numberOfElevators];

    void updateFloors() {
        for(int i = 0; i < numberOfFloors; i++) {
            int maximum = maximumWaiting;
            int length = floors[i].waitingPassengers.size();

            if (length < maximum) {
                poisson_distribution<int> distribution(0.1);
                auto numberOfNewPeople = distribution(gen);
                for (int j = 0; j < numberOfNewPeople && j < maximum - length; j++) {
                    Passenger p;
                    floors[i].waitingPassengers.push_back(p);
                }
            }
        }
    }

public:
    State() {
        for (int i = 0; i < numberOfFloors; i++) {
            int numberOfWaiters = rand() % (maximumWaiting+1);
            for (int j = 0; j < numberOfWaiters; j++) {
                Passenger p;
                while (p.goalFloor == i) {
                    p.goalFloor = rand() % numberOfFloors;
                }
                floors[i].waitingPassengers.push_back(p);
            }
        }
    }

    /**
     * Maximum number of floors: 6.
     * @return
     */
    unsigned int toKey() {
        unsigned int key = 0;

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
            key *= 100;

            for (int j = 0; j < numberOfFloors; j++) {
                bits[j] = elevators[i].goalFloors[j];
            }
            key += bitsToInt(bits,numberOfFloors);
        }
        return key;
    }

    bool isTerminal(){
        // Are all floors empty
        for (int i = 0; i < numberOfFloors; i++) {
            if (floors[i].waitingPassengers.size() > 0) return false;
        }

        // Are all elevators empty
        for (int i = 0; i < numberOfElevators; i++) {
            if (elevators[i].passengers.size() > 0) return false;
        }
        return true;
    }

    int performAction(int numberOfElevator, int action){
        Elevator *e = &elevators[numberOfElevator];
        switch(action) {
            case 0 : // Elevator goes up
                //cout << "Up" << endl;
                if(e->currentFloor != numberOfFloors-1){
                    e->currentFloor++;
                }
                return 0;
            case 1 : // Elevator goes down
                //cout << "Down" << endl;
                if(e->currentFloor != 0){
                    e->currentFloor--;
                }
                return 1;
            case 2 : // Elevator stays at the same floor
                //cout << "Stay" << endl;

                // Passengers leaving the elevator.
                if(e->passengers.size() > 0){
                    auto it = e->passengers.begin();
                    while (it != e->passengers.end()) {
                        if (it->goalFloor == e->currentFloor) {
                            //cout << "Passenger size: " << e->passengers.size();
                            it = e->passengers.erase(it);
//                            cout << " " << e->passengers.size() << endl;
                            e->goalFloors[e->currentFloor] = 0;
                        } else {
                            it++;
                        }
                    }
                }

                // Passengers boarding the elevator.
                while (e->passengers.size() < capacity) {
                    if (floors[e->currentFloor].waitingPassengers.empty()) return 2;
                    auto p = floors[e->currentFloor].waitingPassengers.back();
                    floors[e->currentFloor].waitingPassengers.pop_back();
                    e->goalFloors[p.goalFloor] = 1;
                    e->passengers.push_back(p);
                }
                return 2;
        }
        cout << "Action: " << action << endl;
        exit(24);
    }

    int *updateState(int action) {
        // All elevators perform their actions
        for(int elevator = 0; elevator < numberOfElevators; elevator++) {
            actions[elevator] = performAction(elevator,action);
        }
        return actions;
    };
    void print() {
        // Print divider
        cout << "-------------------" << endl;
        for (int i= 0; i < numberOfElevators; i++) {
            cout << "-------------------" << endl;
        }
        // Print the rest
        for(int i = numberOfFloors-1; i>=0; i--) {
            cout << "         ";
            for (int j = 0; j < floors[i].waitingPassengers.size(); j++) {
                printf("%d ", floors[i].waitingPassengers.at(j).goalFloor);
            }

            for (int nthElevator = 0; nthElevator < numberOfElevators; nthElevator++) {
                Elevator elevator = elevators[nthElevator];
                if (elevator.currentFloor == i) {
                    printf("\t\t");
                    printf("| ");
                    for (int goal = 0; goal < numberOfFloors; goal++) {
                        printf("%1d  ", elevator.goalFloors[goal]);
                    }
                    printf("|");
                }
                printf("\n");
                printf("Floor %d ----------\n\n", i);
            }
        }
    }

    int getReward() {
        if (rewardSystem == 1) {
            return isTerminal();
        } else if (rewardSystem == 2) {
            int people = 0;
            for (int i = 0; i < numberOfFloors; i++) {
                people += floors[i].waitingPassengers.size();
            }
            for (int i = 0; i < numberOfElevators; i++) {
                people += elevators[i].passengers.size();
            }
            return -people;
        } else {
            cerr << "Reward system not recognized." << endl;
        }
    }

    Elevator *getElevators() { return elevators; }
    Floor *getFloors() { return floors; }
};


#endif //SECONDPRACTICAL_STATE_H
