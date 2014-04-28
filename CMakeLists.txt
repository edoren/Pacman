# define cmake version
cmake_minimum_required(VERSION 2.8.10)

# project name
project(Pacman)

# Build type
set(CMAKE_BUILD_TYPE Release) # Choose the type of build (Debug or Release)

# set CMAKE_MODULE_PATH
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/Modules)

# compiler language
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "-std=c++11")
endif()

# add the subdirectories
add_subdirectory(src)
