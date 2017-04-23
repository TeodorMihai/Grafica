#pragma once
#include "GameObject.h"

#include <string>

using namespace std;

class Plane : public GameObject {

public:
	Plane();
	Plane(string name);
	~Plane();

};