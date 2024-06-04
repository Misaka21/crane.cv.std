cmake_minimum_required(VERSION 3.2)
project(crane_cv_std)

set(CMAKE_CXX_STANDARD 17)



add_executable(crane_cv_std main.cpp
        detector.cpp
        detector.h
        main.h)

find_package(OpenCV REQUIRED)
target_link_libraries(crane_cv_std ${OpenCV_LIBS})
target_include_directories(crane_cv_std PRIVATE ${OpenCV_INCLUDE_DIRS} )