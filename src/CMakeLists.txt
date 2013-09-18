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
    ${SRC_DIR}/TileMap.cpp
    ${SRC_DIR}/Sound.cpp
    ${SRC_DIR}/Collision.cpp
)

# set pacman headers
set(PACMAN_HEADERS
    ${HEADER_DIR}/Sprite.hpp
    ${HEADER_DIR}/Pacman.hpp
    ${HEADER_DIR}/Ghost.hpp
    ${HEADER_DIR}/TileMap.hpp
    ${HEADER_DIR}/Sound.hpp
    ${HEADER_DIR}/Collision.hpp
    ${HEADER_DIR}/Input.hpp
    ${HEADER_DIR}/Config.hpp
)

# create game executable
add_executable(${EXECUTABLE_NAME} ${PACMAN_SRC} ${PACMAN_HEADERS})

# set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML 2 COMPONENTS audio graphics window system REQUIRED)
if(SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES})
endif(SFML_FOUND)

set(PYTHON_VERSION 3.3)

# if python3 is installed using Homebrew
if(APPLE)
    foreach(SUFIX "0" "1" "2" "3")
        set(POSIBLE_PYTHON_LIBRARY /usr/local/Cellar/python3/${PYTHON_VERSION}.${SUFIX}/Frameworks/Python.framework/Versions/${PYTHON_VERSION}/lib/python${PYTHON_VERSION}/config-${PYTHON_VERSION}m/libpython${PYTHON_VERSION}m.dylib)
        set(POSIBLE_PYTHON_INCLUDE_DIR /usr/local/Cellar/python3/${PYTHON_VERSION}.${SUFIX}/Frameworks/Python.framework/Versions/${PYTHON_VERSION}/include/python${PYTHON_VERSION}m)
        if(EXISTS ${POSIBLE_PYTHON_LIBRARY})
            set(PYTHON_LIBRARY ${POSIBLE_PYTHON_LIBRARY})
        endif()
        if(EXISTS ${POSIBLE_PYTHON_INCLUDE_DIR})
            set(PYTHON_INCLUDE_DIR ${POSIBLE_PYTHON_INCLUDE_DIR})
        endif()
    endforeach()
endif()

find_package(PythonLibs ${PYTHON_VERSION} REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})
target_link_libraries(${EXECUTABLE_NAME} ${PYTHON_LIBRARIES})

# find_package(Lua 5.2 REQUIRED)
# include_directories(${LUA_INCLUDE_DIR})
# target_link_libraries(${EXECUTABLE_NAME} ${LUA_LIBRARIES})
