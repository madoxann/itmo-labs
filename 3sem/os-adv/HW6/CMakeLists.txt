cmake_minimum_required(VERSION 3.24)
project(HW6)

set(CMAKE_CXX_STANDARD 20)

include_directories(.)

add_executable(HW6
        pe-defines.h
        pe-parser.cpp
#        ConsoleApplication1.cpp
        return_codes.h
        )
