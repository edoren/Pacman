# define cmake version
cmake_minimum_required(VERSION 2.8)

# project name
project(Pacman)

# set CMAKE_MODULE_PATH
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/Modules)

# compiler language
set (CMAKE_CXX_FLAGS "-std=c++0x -g3 -Wall -O0")

# add the subdirectories
add_subdirectory(src)
