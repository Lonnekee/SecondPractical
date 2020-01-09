#ifndef SECONDPRACTICAL_STATE_H
#define SECONDPRACTICAL_STATE_H

#include "Elevator.h"
#include "Floor.h"
#include "Util.h"
#include "Constants.h"
#include <float.h>

using namespace std;

static std::random_device rd;
static std::mt19937 gen(rd());

class State {
private:
    double epsilon;
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
    double reward = 0;

    State(double epsilon) : epsilon(epsilon) {
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

    void giveRewards();

    bool areFloorsEmpty(){
        for (int i = 0; i < numberOfFloors; i++) {
            if (floors[i].waitingPassengers.size() > 0) return false;
        }
        return true;
    }

    int performAction(int numberOfElevator, int action){
        Elevator *e = &elevators[numberOfElevator];
        switch(action) {
            case 0 : // Elevator goes up
//                cout << "Up" << endl;
                if(e->currentFloor != numberOfFloors-1){
                    e->currentFloor++;
                }
                return 0;
            case 1 : // Elevator goes down
//                cout << "Down" << endl;
                if(e->currentFloor != 0){
                    e->currentFloor--;
                }
                return 1;
            case 2 : // Elevator stays at the same floor
//                cout << "Stay" << endl;

                // Passengers leaving the elevator.
                if(e->passengers.size() > 0){
                    auto it = e->passengers.begin();
                    while (it != e->passengers.end()) {
                        if (it->goalFloor == e->currentFloor) {
                            //cout << "Passenger size: " << e->passengers.size();
                            it = e->passengers.erase(it);
//                            cout << " " << e->passengers.size() << endl;
                            e->goalFloors[e->currentFloor] = 0;

                            // Add reward for every person that arrives at their destination
                            reward++;
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
        // Setting the reward of this turn back to zero
        reward = 0;

        // All elevators perform their actions
        for(int elevator = 0; elevator < numberOfElevators; elevator++) {
            actions[elevator] = performAction(elevator,action);
        }
//        updateFloors();

        // Punishing for waiting passengers
        //for (int i = 0; i < numberOfFloors; i++) {
        //   reward -= floors[i].waitingPassengers.size();
        //}

        return actions;
    };
    void print() {
        // Print divider
        cout << "---------------------";
        for (int i= 0; i < numberOfElevators; i++) {
            cout << "---------------------";
        }
        cout << endl;

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

    int getReward() { return reward; }

    Elevator *getElevators() { return elevators; }
    Floor *getFloors() { return floors; }
};


#endif //SECONDPRACTICAL_STATE_H
