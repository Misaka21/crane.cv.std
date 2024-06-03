cmake_minimum_required(VERSION 3.28)
project(crane_cv_std)

set(CMAKE_CXX_STANDARD 20)

add_executable(crane_cv_std main.cpp
        detector.cpp
        detector.h)
