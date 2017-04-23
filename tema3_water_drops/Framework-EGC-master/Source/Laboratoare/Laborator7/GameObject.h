#pragma once

#include <string>
#include <Core/Engine.h>

using namespace std;

class GameObject {

public:

	string nameObject;
	string shader;
	glm::vec3 position;
	float scale;
	glm::vec3 color;

	GameObject();
	GameObject(string nameObject, string shader = "SimpleShader", glm::vec3 position = glm::vec3(0),
		float scale = 1.0, glm::vec3 color = glm::vec3(0.5));
	~GameObject();

	glm::mat4 computeMatrix();

};