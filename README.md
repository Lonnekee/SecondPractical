# Using reinforcement learning for elevator control
By Stijn de Vries and Lonneke Pulles

 ## How to compile the program
 This project is a CMake-based project, meaning that building and compiling
 the project is done using the file CMakeLists.txt.
 It was made on Windows, using CLion.
 
 On Linux distributions, entering the following on the command line should probably compile the program:
 
 mkdir build
 
 cd build
 
 cmake ../SecondPractical
 
 cmake --build . 
 
 
 ## How to start the program
 The main function is included in the file called main.cpp. On Windows, CLion
 simply shows a button to run the program. No program arguments need to be added.
 
 On Linux distributions, you can navigate to the directory where SecondPractical was
 build and simply enter "SecondPractical" on the command line.
 
 ## How the results can be seen
 
 The program initially asks the user which algorithm it wants to use.
 After entering either 1, 2 or 3 for Q-learning, Sarsa or a standard elevator,
 respectively, the program will run.
 
 The results are printed to the console: The progress of the algorithm and the average epochs needed to reach the terminal state during the last 100 repetitions is printed to the console after every 100 repetitions.