#include "GameObject.h"
#include "Plane.h"

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>

using namespace std;

Plane::Plane(string name) : GameObject(name) {

}

Plane::Plane() { }

Plane::~Plane() {

}