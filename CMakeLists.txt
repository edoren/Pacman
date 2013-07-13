# define cmake version
cmake_minimum_required(VERSION 2.8)

# Build type
if(CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_BUILD_TYPE Debug)
endif()

# project name
project(Pacman)

# set CMAKE_MODULE_PATH
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/Modules)

# SFML Root
if(WIN32)
    set(SFML_ROOT ${PROJECT_SOURCE_DIR}/SFML-2.0)
endif(WIN32)

# compiler language
# add_definitions("-Wall")
if(APPLE)
    set(CMAKE_CXX_FLAGS "-std=c++0x -stdlib=libc++")
else()
    set(CMAKE_CXX_FLAGS "-std=c++0x")
endif()

# add the Pacman header path
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/include)

# add the subdirectories
add_subdirectory(src)
