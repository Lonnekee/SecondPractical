#ifndef SECONDPRACTICAL_CONSTANTS_H
#define SECONDPRACTICAL_CONSTANTS_H

#define UP 0
#define DOWN 1
#define STAY 2

static const int numberOfFloors = 6;
static const int numberOfElevators = 1;
static const int maximumWaiting = 4;
static const int capacity = 2;
static double alpha = 0.19;
static double epsilon = 0.215;
static double gamma = 0.99;
/* Reward systems:
 *  1: reward 1 at the end, 0 otherwise.
 *  2: negative rewards for number of waiting people, both on the floors and in the elevator(s).
 */
static const int rewardSystem = 1;

#endif //SECONDPRACTICAL_CONSTANTS_H
