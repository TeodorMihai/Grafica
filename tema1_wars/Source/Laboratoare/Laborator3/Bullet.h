#pragma once

#include <Component/SimpleScene.h>
#include <string>
#include <Core/Engine.h>

class Bullet
{

public:
	glm::vec3 centroid;
	float orientation;
	float velocity;
	Bullet();
	Bullet(glm::vec3 _centroid, float _orientation);
	~Bullet();

	bool operator<(const Bullet& b) const;
};