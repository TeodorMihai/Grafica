#pragma once

#include "Life.h"
#include <Core/Engine.h>
#include "Transform2D.h"

Life::Life() { }
Life::Life(glm::vec3 _centroid, float _size) : centroid(_centroid), size(_size) { }
Life::~Life() { }

bool Life::operator<(const Life& b) const {
	if (b.centroid[0] == this->centroid[0]) {
		if (b.centroid[1] == this->centroid[1])
			return false;
		else
			return b.centroid[1] < this->centroid[1];
	}
	else return b.centroid[0] < this->centroid[0];
}