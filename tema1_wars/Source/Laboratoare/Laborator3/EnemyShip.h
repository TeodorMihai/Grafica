#pragma once

#include <Component/SimpleScene.h>
#include <string>
#include <Core/Engine.h>
#include "Ship.h"


class EnemyShip : public Ship {

	public:
		EnemyShip();
		EnemyShip(glm::vec3 _centroid, float _sideLength, float _velocity, float _orientation, int _type);
		~EnemyShip();
		bool operator<(const EnemyShip& e) const;
		int typeShipe;
		float timeOfHitting = 0;
};