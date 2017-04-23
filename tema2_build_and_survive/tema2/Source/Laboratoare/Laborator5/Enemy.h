#pragma once

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>


#include "GameObject.h"
class Enemy : public GameObject {

public:
	const static int FIRST = 0;
	const static int SECOND = 1;
	const static int THIRD = 2;
	const static int FOURTH = 3;

	float timeSinceDeath;
	float dimension;
	int lives;
	int period;
	Enemy();

};
