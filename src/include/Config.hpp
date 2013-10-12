#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <iostream>

#ifdef _WIN32
    #define CONTROL LControl
    #define PATH_SEPARATOR '\\'
#elif __linux__
    #define CONTROL LControl
    #define PATH_SEPARATOR '/'
#elif __APPLE__
    #define CONTROL LSystem
    #define PATH_SEPARATOR '/'
#endif

#define PACMAN_WINS true
#define PACMAN_LOSE false

// Configuration definitions
#define WINDOW_WIDTH 224
#define WINDOW_HEIGHT 288

#define FRAME_RATE 60

extern std::string pacmanPath;

#endif // CONFIG_HPP
