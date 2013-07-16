# include some functional macros
include(${PROJECT_SOURCE_DIR}/cmake/Macros.cmake)

# set the Pacman source and header directories
set(SRC_DIR ${PROJECT_SOURCE_DIR}/src)
set(HEADER_DIR ${PROJECT_SOURCE_DIR}/src/include)

# set the lua compiled scripts directory
set(LUA_OBJ_DIR ${PROJECT_SOURCE_DIR}/src/data)

# Executable name
set(EXECUTABLE_NAME pacman)

# set pacman sources
set(PACMAN_SRC
    ${SRC_DIR}/main.cpp
    ${SRC_DIR}/Sprite.cpp
    ${SRC_DIR}/Pacman.cpp
    ${SRC_DIR}/Ghost.cpp
    ${SRC_DIR}/Collision.cpp
)

# set pacman headers
set(PACMAN_HEADERS
    ${HEADER_DIR}/Sprite.hpp
    ${HEADER_DIR}/Pacman.hpp
    ${HEADER_DIR}/Ghost.hpp
    ${HEADER_DIR}/Collision.hpp
    ${HEADER_DIR}/Input.hpp
    ${HEADER_DIR}/Config.hpp
)

# create game executable
add_executable(${EXECUTABLE_NAME} ${PACMAN_SRC} ${PACMAN_HEADERS})

# set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML 2 COMPONENTS graphics window system REQUIRED)
if(SFML_FOUND)
    # including directories
    include_directories(${SFML_INCLUDE_DIR})
    # linking libraries
    target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES})
endif(SFML_FOUND)

find_package(Lua 5 REQUIRED)
