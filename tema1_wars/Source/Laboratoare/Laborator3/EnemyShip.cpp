#include "EnemyShip.h"
#include <Core/Engine.h>
#include "Transform2D.h"

EnemyShip::EnemyShip() { }
EnemyShip::EnemyShip(glm::vec3 _centroid, float _sideLength, float _velocity, float _orientation, int _type) :
	Ship(_centroid, _sideLength, _velocity, _orientation) , typeShipe(_type) { }
EnemyShip::~EnemyShip() { }

bool EnemyShip::operator<(const EnemyShip& e) const {

	if (e.centroid[0] == this->centroid[0]) {
		if (e.centroid[1] == this->centroid[1])
			return false;
		else
			return e.centroid[1] < this->centroid[1];
	}
	else return e.centroid[0] < this->centroid[0];
}