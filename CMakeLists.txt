# define cmake version
cmake_minimum_required(VERSION 2.8)

# project name
project(Pacman)

# set CMAKE_MODULE_PATH
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/Modules)

# Build type
if(CMAKE_BUILD_TYPE STREQUAL "")
 	set(CMAKE_BUILD_TYPE Debug)
endif()

# SFML Root
if(WIN32)
	set(SFML_ROOT ${PROJECT_SOURCE_DIR}/SFML-2.0)
endif(WIN32)

# compiler language
# add_definitions("-Wall")
if(APPLE)
  set (CMAKE_CXX_FLAGS "-std=c++0x -stdlib=libc++")
endif(APPLE)

# add the Pacman header path
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/header)

# add the subdirectories
add_subdirectory(src)
