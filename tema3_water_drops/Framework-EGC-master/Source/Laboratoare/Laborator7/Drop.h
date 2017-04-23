#pragma once
#include "GameObject.h"

#include <string>

using namespace std;

class Drop : public GameObject {

public:

	Drop();
	Drop(string name);
	Drop(string name, int direction, float mass);
	~Drop();

	float velocity;
	int direction;
	float mass;
	int jumps = 0;
};