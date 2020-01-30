#include <iostream>
#include "Elevator.h"
#include "LookupTable.h"
#include "Algorithms.h"
#include <math.h>
#include "Plot.h"


using namespace std;

int main() {
    int numberOfStates = pow(2, numberOfFloors) * pow((numberOfFloors * pow(2, numberOfFloors)), numberOfElevators);
    cout << "Number of states: " << numberOfStates << endl;

    int algorithm;
    cout << "Which algorithm do you want to use? 1 : Q-learning, 2 : Sarsa, 3 : normal elevator" << endl;
    cin >> algorithm;

    while (!(1 <= algorithm || algorithm <= 3)) {
        cout << "Please enter 1 or 2." << endl;
        cout << algorithm << endl;
        cin >> algorithm;
    }

    int maxRepetitions = 20000;
    int maxEpochs = 10000;
    LookupTable lookupTable;
    Algorithms td;

    plot();

    if (algorithm == 1) { // Q-learning
        alpha = 0.12;
        epsilon = 0.13;
        gamma = 0.66;
        td.runQlearning(maxRepetitions, maxEpochs, lookupTable);
    } else if (algorithm == 2) { // SARSA
        alpha = 0.19;
        epsilon = 0.3;
        gamma = 0.99;
        td.runSarsa(maxRepetitions, maxEpochs, lookupTable);
    } else if (algorithm == 3) {
        td.runNormal(maxRepetitions, maxEpochs);
    } else {
        cerr << "Number not associated with an algorithm." << endl;
    }
    return 0;


    /** Code for parameter sweep.
     *
     * ofstream myfile("Parameter Sweep/SarsaEpsilonSecond");
    myfile << "Number of floors: " << numberOfFloors << " maximum waiting: " << maximumWaiting << endl;
    myfile << "(optimized) Alpha: " << alpha << endl;
    myfile << "Gamma: " << gamma << endl;
     *
     * auto start = std::chrono::system_clock::now();
     *
     * Every loop:
       //Print results
        if (repetitions % 100 == 0) {
            results[repetitions / 100 - 1] = averageEpoch / 100;
            //cout << "Average finishing epoch per waiting person: " << averageEpoch / 100 << endl;
            averageEpoch = 0.0;
        }

        if (repetitions > 10000) {
            auto end = std::chrono::system_clock::now();
            std::chrono::duration<double> elapsed_seconds = end - start;
            if (elapsed_seconds.count() > 180.00) {
                totalValue = -1;
                break;
            }
        }


        if(totalValue == -1){
            myfile << "With epsilon = " << epsilon << ", the program took more than 3 minutes and was terminated." << endl;
        }
        totalValue = 0;
        for (int i = (maxRepetitions / 100) - 100; i < (maxRepetitions / 100); i++) {
            totalValue += results[i];
        }
        cout << "Epsilon: " << epsilon << " with the average of last 100 reps: " << totalValue / 100 << endl;
        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = end - start;
        std::time_t end_time = std::chrono::system_clock::to_time_t(end);

        if (myfile.is_open()) {
            myfile << "With epsilon = " << epsilon << ", performance = " << totalValue / 100 << " and elapsed time: " << elapsed_seconds.count() << endl;
        } else cout << "Unable to open file";

     */
}
