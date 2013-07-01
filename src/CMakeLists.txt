# Executable name
set(EXECUTABLE_NAME pacman)

# set pacman sources
set(PACMAN_SRC
	main.cpp
	Sprite.cpp
	Pacman.cpp
	Collision.cpp
)

set(PACMAN_HEADERS
	Sprite.hpp
	Pacman.hpp
	Collision.h
	Input.hpp
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
