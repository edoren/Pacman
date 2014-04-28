# set the Pacman source and header directories
set(SRC_DIR ${PROJECT_SOURCE_DIR}/src)

# Executable name
set(EXECUTABLE_NAME pacman)

# A
include_directories(${SRC_DIR})

# set pacman sources
set(PACMAN_SRC
    ${SRC_DIR}/Engine/GameEngine.cpp
    ${SRC_DIR}/Engine/ResourceManager.cpp
    ${SRC_DIR}/Engine/GameState.cpp
    ${SRC_DIR}/Engine/AnimatorJson.cpp
    ${SRC_DIR}/Engine/Utils/Clock.cpp
    ${SRC_DIR}/IntroState.cpp
    ${SRC_DIR}/MenuState.cpp
    ${SRC_DIR}/PlayState.cpp
    ${SRC_DIR}/Pacman.cpp
    ${SRC_DIR}/Ghost.cpp
    ${SRC_DIR}/Main.cpp
)

# set pacman headers
set(PACMAN_HEADERS
    ${SRC_DIR}/Engine/GameEngine.hpp
    ${SRC_DIR}/Engine/ResourceManager.hpp
    ${SRC_DIR}/Engine/GameState.hpp
    ${SRC_DIR}/Engine/AnimatorJson.hpp
    ${SRC_DIR}/Engine/Utils/Clock.hpp
    ${SRC_DIR}/MenuState.hpp
    ${SRC_DIR}/IntroState.hpp
    ${SRC_DIR}/PlayState.hpp
    ${SRC_DIR}/Pacman.hpp
    ${SRC_DIR}/Ghost.hpp
)

# create game executable
add_executable(${EXECUTABLE_NAME} ${PACMAN_SRC} ${PACMAN_HEADERS})

# Project propeties for IDEs
set_target_properties(pacman PROPERTIES 
                        PROJECT_LABEL "PACMAN"
                        RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})


# Finding and link libraries
if(OS_WINDOWS)
    find_package(SFML 2.1 COMPONENTS main audio graphics window system REQUIRED)
else()
    find_package(SFML 2.1 COMPONENTS audio graphics window system REQUIRED)
endif()
if(SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES})
endif(SFML_FOUND)

target_link_libraries(${EXECUTABLE_NAME} STP jansson)

if(OS_LINUX)
    target_link_libraries(${EXECUTABLE_NAME} dl)
endif()
