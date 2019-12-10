cmake_minimum_required(VERSION 3.15)
project(OS_Project_3 C)

set(CMAKE_C_STANDARD 99)
SET(CMAKE_CXX_FLAGS -pthread)
include_directories(./include)

add_executable(mystation include/mystation.h include/files.h src/mystation.c src/files.c)
add_executable(bus include/bus.h src/bus.c)
add_executable(station-manager include/station-manager.h src/station-manager.c)
add_executable(comptroller include/comptroller.h src/comptroller.c)

target_link_libraries(mystation pthread)
target_link_libraries(bus pthread)
target_link_libraries(station-manager pthread)