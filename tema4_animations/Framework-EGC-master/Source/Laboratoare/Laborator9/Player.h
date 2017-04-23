#pragma once

#include <vector>
#include <Core/Engine.h>

#include "Animation.h"
using namespace std;

class Player {

public:
	glm::vec3 pos;
	int lifes;
	int orientation;

	int cycles = 0;

	string currentAnimation;
	int indexAnimation;

	vector<Animation> animations;

	Player();
	Player(glm::vec3 pos, int lifes, int orientation);
	~Player();


};