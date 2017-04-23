#include "Ship.h"
#include <Core/Engine.h>
#include "Transform2D.h"
#include "GeometryWars.h"

Ship::Ship() : lives(GeometryWars::livesEnemies) { }

Ship::Ship(glm::vec3 _centroid, float _sideLength, float _velocity, float _orientation)
	: centroid(_centroid), sideLength(_sideLength),
	velocity(_velocity), orientation(_orientation) , lives(GeometryWars::livesEnemies) { }

Ship::~Ship() { }

