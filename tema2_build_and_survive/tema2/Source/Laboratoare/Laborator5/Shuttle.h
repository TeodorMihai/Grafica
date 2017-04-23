#pragma once

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>

#include "GameObject.h"

class Shuttle : public GameObject {

public:

	const static int PISTOL = 0;
	const static int RIFLE = 1;
	const static int LAUNCHER = 2;

	int lives;
	float timeSinceDeath;
	int mode;
	bool isAlive;
	Shuttle();

};
