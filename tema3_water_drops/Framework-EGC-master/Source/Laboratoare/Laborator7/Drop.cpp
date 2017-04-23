#include "GameObject.h"
#include "Drop.h"

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>

using namespace std;

Drop::Drop(string name) : GameObject(name) { }

Drop::Drop(string name, int direction, float mass) : GameObject(name), velocity(0),
			direction(direction), mass(mass) { }
Drop::Drop() { }

Drop::~Drop() {

}