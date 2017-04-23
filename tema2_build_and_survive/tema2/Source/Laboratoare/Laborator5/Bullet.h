#pragma once

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>

#include "GameObject.h"

class Bullet : public GameObject {

public:

	glm::vec3 direction3;
	int power;
	float distance;
	float timeSinceLaunch;
	Bullet();
};
