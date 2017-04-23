#include "Bullet.h"
#include <Core/Engine.h>
#include "Transform2D.h"

Bullet::Bullet() { }
Bullet::Bullet(glm::vec3 _centroid, float _orientation) : centroid(_centroid), orientation(_orientation) { }
Bullet::~Bullet() { }

bool Bullet::operator<(const Bullet& b) const {
	if (b.centroid[0] == this->centroid[0]) {
		if (b.centroid[1] == this->centroid[1])
			return false;
		else
			return b.centroid[1] < this->centroid[1];
	}
	else return b.centroid[0] < this->centroid[0];
}