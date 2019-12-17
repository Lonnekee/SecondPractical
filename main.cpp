#include <iostream>
#include "Elevator.h"
#include "Floor.h"
#include <math.h>

using namespace std;

void printState(int numFloor, Floor *floorArray,Elevator elevator){
    for(int i = numFloor-1; i>=0; i--){
        for(int j = 0; j<floorArray[i].maximumWaiting; j++){
            if(floorArray[i].waitingPassengers[j].goalFloor != -1) {
                printf("%d ", floorArray[i].waitingPassengers[j].goalFloor);
            }
        }
        printf("  ");
        if(elevator.currentFloor == i){
            printf("| %d %d |", elevator.goalFloors[0], elevator.goalFloors[1]);
        }
        printf("\n");
        printf("--------\n\n");
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

//      To calculate the number of states:
//    int maximumWaiting = 4, numberOfFloors = 3, capacity = 2, numberOfLifts = 2;
//    int numberOfStates = pow(maximumWaiting+1,numberOfFloors) * pow(((capacity+1) * numberOfFloors * pow(2, numberOfFloors)), numberOfLifts);
//    cout << numberOfStates << endl;

//    Elevator e;
//    Floor f0, f1, f2, f3, f4;
//    // Each floor should have a different random distribution for adding waiting people
//    int maxEpochs = 1000;
//
//    for (int epoch = 1; epoch < maxEpochs; epoch++) {
//        // Lifts get rewards
//
//
//        // Lifts choose an action.
//
//        // Update state: lift's action has been performed (go to floor, some people step out),
//        // randomly add waiting people on each floor.
//    }

    int numberOfFloors = 5;
    Elevator elevatorOne;
    Floor *floorArray = new Floor[numberOfFloors];
    printState(numberOfFloors,floorArray,elevatorOne);
    delete(floorArray);
    return 0;
}