#include "Pacman.hpp"

// Constructor
Pacman::Pacman() : lastPos() {
	Collision::CreateTextureAndBitmask(this->texture, "resources/pacmanSprites2.png");
	this->setTexture(this->texture);
	this->setTextureRect(sf::IntRect(43, 3, 14, 14));
	this->setPosition(sf::Vector2f(95, 205));

	this->mouth = true;
	this->animation = true;
	this->setFrame(0);
	this->setSpeed(sf::Vector2f(-1,0));
}

//Destructor
Pacman::~Pacman() {}


sf::Vector2f Pacman::getLastPos() {
	return this->lastPos;
}

bool Pacman::getAnimation() {
	return this->animation;
}

bool Pacman::getMouthState() {
	return this->mouth;
}

void Pacman::setMouthState(bool mouth) {
	this->mouth = mouth;
}

void Pacman::setLastPos(sf::Vector2f lastPos) {
	this->lastPos = lastPos;
}

void Pacman::setAnimation(bool animation) {
	this->animation = animation;
}
