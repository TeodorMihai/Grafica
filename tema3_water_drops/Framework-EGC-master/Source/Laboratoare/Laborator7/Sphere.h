#pragma once
#include "GameObject.h"

#include <string>

using namespace std;

class Sphere : public GameObject {

public:
	Sphere();
	Sphere(string name);
	~Sphere();

};