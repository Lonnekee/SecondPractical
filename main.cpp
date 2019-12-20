#include <iostream>
#include "Elevator.h"
#include "Floor.h"
#include "State.h"
#include <math.h>

using namespace std;

void printState(int numFloor, Floor *floorArray, Elevator elevator){
    cout << endl;
    for(int i = numFloor-1; i>=0; i--){
        cout << "         " ;
        for(int j = 0; j<floorArray[i].waitingPassengers.size(); j++){
            if(floorArray[i].waitingPassengers[j].goalFloor != -1) {
                printf("%d ", floorArray[i].waitingPassengers[j].goalFloor);
            }
        }

        if(elevator.currentFloor == i){
            printf("\t\t");
            printf("| ");
            for (int goal = 0; goal < elevator.goalFloors.size(); goal++) {
                printf("%1d  ",elevator.goalFloors[goal]);
            }
            for (int rest = 0; rest < elevator.capacity - elevator.goalFloors.size(); rest++) {
                printf("   ");
            }
            printf("|");
        }
        printf("\n");
        printf("Floor %d ----------\n\n",i);
    }
}

int main() {
    /*
     * number of states = (maximumWaiting+1 ^ numberOfFloors) * ( ( (capacity+1) * numberOfFloors * 2 ^ numberOfFloors) ^ numberOfLifts)
     *
     * Valid options:
     * - 648000: int maximumWaiting = 4, numberOfFloors = 3, capacity = 2, numberOfLifts = 2;
     * - 819199: int maximumWaiting = 3, numberOfFloors = 5, capacity = 4, numberOfLifts = 1;
     */

//  To calculate the number of states:
    int maximumWaiting = 4, numberOfFloors = 3, capacity = 2, numberOfLifts = 1;
    int numberOfStates = pow(maximumWaiting+1,numberOfFloors) * pow(((capacity+1) * numberOfFloors * pow(2, numberOfFloors)), numberOfLifts);
    cout << "Number of states: " << numberOfStates << endl;

    State s;
    // Each floor should have a different random distribution for adding waiting people
    int maxEpochs = 1000;

    for (int epoch = 1; epoch < maxEpochs; epoch++) {
        // Lifts get rewards
        s.giveRewards();

        // Lifts choose an action.
        s.selectActions();

        // Update state: lift's action has been performed (go to floor, some people step out),
        // randomly add waiting people on each floor.
        s.updateState();
    }

    printState(5,s.getFloors(),s.getElevators()[0]);

    return 0;
}
