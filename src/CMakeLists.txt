# set the Pacman source and header directories
set(SRC_DIR ${PROJECT_SOURCE_DIR}/src)

# Executable name
set(EXECUTABLE_NAME pacman)

# Finding the libraries
if(WIN32)
    find_package(SFML 2.1 COMPONENTS main audio graphics window system)
else()
    find_package(SFML 2.1 COMPONENTS audio graphics window system)
endif()
if(SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
else()
    message("Building SFML from source.")
    set(SFML_DIRECTORY ${PROJECT_SOURCE_DIR}/extlibs/SFML)

    set(SFML_FOUND true)
    set(SFML_INCLUDE_DIR ${SFML_DIRECTORY}/include)

    set(BUILD_SHARED_LIBS FALSE)
    add_definitions(-DSFML_STATIC)

    add_subdirectory(${SFML_DIRECTORY} ${PROJECT_BINARY_DIR}/extlibs/SFML EXCLUDE_FROM_ALL)
    if(WIN32)
        set(SFML_LIBRARIES sfml-main sfml-audio sfml-graphics sfml-window sfml-system)
    else()
        set(SFML_LIBRARIES sfml-audio sfml-graphics sfml-window sfml-system)
    endif()

    include_directories(${SFML_INCLUDE_DIR})
endif()

find_package(Jansson)
if(JANSSON_FOUND)
    include_directories(${JANSSON_INCLUDE_DIR})
else()
    message("Building Jansson from source.")
    set(JANSSON_DIRECTORY ${PROJECT_SOURCE_DIR}/extlibs/jansson)

    set(JANSSON_BUILD_SHARED_LIBS OFF)

    add_subdirectory(${JANSSON_DIRECTORY} ${PROJECT_BINARY_DIR}/extlibs/jansson EXCLUDE_FROM_ALL)

    include_directories(${PROJECT_BINARY_DIR}/extlibs/jansson/include)
    set(JANSSON_LIBRARY jansson)
endif()

find_package(STP)
if(STP_FOUND)
    include_directories(${STP_INCLUDE_DIR})
else()
    message("Building STP from source.")
    set(STP_DIRECTORY ${PROJECT_SOURCE_DIR}/extlibs/STP)

    set(STP_SHARED_LIBS FALSE)
    add_definitions(-DSTP_STATIC)

    add_subdirectory(${STP_DIRECTORY} ${PROJECT_BINARY_DIR}/extlibs/STP EXCLUDE_FROM_ALL)

    include_directories(${STP_DIRECTORY}/include)
    set(STP_LIBRARY STP)
endif()

# Pacman includes
include_directories(${SRC_DIR})

# set pacman sources
set(PACMAN_SRC
    ${SRC_DIR}/Engine/GameEngine.cpp
    ${SRC_DIR}/Engine/Settings.cpp
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
    ${SRC_DIR}/Engine/Settings.hpp
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

# compiler language
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "-std=c++11")
endif()

# create game executable
if(HIDE_CMD)
    add_executable(${EXECUTABLE_NAME} WIN32 ${PACMAN_SRC} ${PACMAN_HEADERS})
else()
    add_executable(${EXECUTABLE_NAME} ${PACMAN_SRC} ${PACMAN_HEADERS})
endif()

target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES})
target_link_libraries(${EXECUTABLE_NAME} ${JANSSON_LIBRARY})
target_link_libraries(${EXECUTABLE_NAME} ${STP_LIBRARY})

# Project propeties for IDEs
set_target_properties(${EXECUTABLE_NAME} PROPERTIES
                      PROJECT_LABEL "PACMAN"
                      RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})
