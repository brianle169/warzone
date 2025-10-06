cmake_minimum_required(VERSION 4.0)
project(warzone)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(warzone
        Orders.cpp
        Orders.h
        OrdersDriver.cpp)
