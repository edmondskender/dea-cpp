cmake_minimum_required(VERSION 3.29)
project(DEA)

set(CMAKE_CXX_STANDARD 20)

add_executable(DEA main.cpp
        dea.h
        dea.cpp)
