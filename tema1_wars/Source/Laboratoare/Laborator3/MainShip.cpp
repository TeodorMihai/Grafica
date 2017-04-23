#include "MainShip.h"
#include <Core/Engine.h>
#include "Transform2D.h"

MainShip::MainShip() : moveUp(false), moveDown(false), moveLeft(false), moveRight(false) { }

MainShip::MainShip(glm::vec3 _centroid, float _sideLength, float _velocity, float _orientation) :
	Ship(_centroid, _sideLength, _velocity, _orientation),
	moveUp(false), moveDown(false), moveLeft(false), moveRight(false) { }

MainShip::~MainShip() { }
void MainShip::computeNextShip(float deltaTime) {

	if (moveDown == true)
		centroid[1] -= deltaTime * velocity;
	if (moveUp == true)
		centroid[1] += deltaTime * velocity;
	if (moveLeft == true)
		centroid[0] -= deltaTime * velocity;
	if (moveRight == true)
		centroid[0] += deltaTime * velocity;
}

