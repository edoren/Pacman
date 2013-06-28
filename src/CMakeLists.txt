# set pacman sources
set(SRC_PACMAN
	main.cpp
	Sprite.cpp
	Pacman.cpp
	Collision.cpp
)

# create game executable
add_executable(pacman ${SRC_PACMAN})

find_package(SFML 2.0 REQUIRED)

# SFML libs to be liked
set(SFML_LIBS
	sfml-graphics
	sfml-window
	sfml-system
)

# linking libraries
target_link_libraries(pacman ${SFML_LIBS})
