# define cmake version
cmake_minimum_required(VERSION 2.8)

# project name
project(Pacman)

# Build type
set(CMAKE_BUILD_TYPE Release) # Choose the type of build (Debug or Release)

# include the configuration and macros file
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/Config.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/Macros.cmake)

# set CMAKE_MODULE_PATH
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/Modules)

# SFML Root
set(SFML_ROOT ${PROJECT_SOURCE_DIR}/SFML)

if(OS_WINDOWS)
    set(SFML_STATIC_LIBRARIES TRUE) # TRUE to link SFML staticaly - Only Windows
endif()

# compiler language
if(COMPILER_CLANG)
    set(CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++")
elseif(COMPILER_GCC)
    set(CMAKE_CXX_FLAGS "-std=c++11")
endif()

# add the Pacman header path
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/include)

# add the subdirectories
add_subdirectory(src)
