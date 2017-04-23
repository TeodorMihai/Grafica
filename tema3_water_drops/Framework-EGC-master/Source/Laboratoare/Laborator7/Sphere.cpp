#include "GameObject.h"
#include "Sphere.h"

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>

using namespace std;

Sphere::Sphere(string name) : GameObject(name)  {

}

Sphere::Sphere() { }

Sphere::~Sphere() {

}