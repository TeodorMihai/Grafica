#pragma once

#include <Component/SimpleScene.h>
#include <string>
#include <Core/Engine.h>

class Ship
{

public:

	glm::vec3 centroid;
	float sideLength;
	float velocity;
	float orientation;
	int lives;

	Ship();
	Ship(glm::vec3 centroid, float sideLength, float velocity, float orientation);
	~Ship();
	float getX() { return centroid[0]; }
	float getY() { return centroid[1]; }
};

