cmake_minimum_required(VERSION 3.15)
project(SecondPractical)

set(CMAKE_CXX_STANDARD 17)

add_executable(SecondPractical main.cpp Elevator.h Passenger.h Floor.h State.h LookupTable.h Util.h Constants.h Plot.h)

MATH(EXPR stack_size "16 * 1024 * 1024") # 16 Mb
set(CMAKE_EXE_LINKER_FLAGS "-Wl,--stack,${stack_size}")