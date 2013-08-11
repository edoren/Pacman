#define __CONFIG_H__
#ifdef __CONFIG_H__

#include <string>

#if defined(_WIN32) || defined(__linux__)
    #define CONTROL LControl
#elif __APPLE__
    #define CONTROL LSystem
#endif

// Configuration definitions
#define WINDOW_WIDTH 224
#define WINDOW_HEIGHT 288

#define FRAME_RATE 60

extern std::string pacmanPath;

#endif // __CONFIG_H__
