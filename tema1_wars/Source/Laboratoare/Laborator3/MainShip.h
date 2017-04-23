#pragma once

#include <Component/SimpleScene.h>
#include <string>
#include <set>
#include <Core/Engine.h>
#include "Ship.h"
#include "Bullet.h"


class MainShip : public Ship {

	public:

		bool moveUp;
		bool moveDown;
		bool moveLeft;
		bool moveRight;

		std::set<Bullet> bullets;

		MainShip();
		MainShip(glm::vec3 _centroid, float _sideLength, float _velocity, float _orientation);
		~MainShip();
		void computeNextShip(float deltaTime);
};