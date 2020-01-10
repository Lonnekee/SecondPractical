cmake_minimum_required(VERSION 3.15)
project(SecondPractical)

set(CMAKE_CXX_STANDARD 17)

add_executable(SecondPractical main.cpp Elevator.h Passenger.h Floor.h State.h LookupTable.h Util.h Constants.h)