#include "GameObject.h"

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>

using namespace std;

GameObject::GameObject(string nameObject, string shader, glm::vec3 position,
	float scale, glm::vec3 color) : nameObject(nameObject), shader(shader),
	position(position), scale(scale), color(color) { }

GameObject::~GameObject() { }

GameObject::GameObject() : position(glm::vec3(0)), scale(1.0f)  { }

glm::mat4 GameObject::computeMatrix() {

	glm::mat4 m(1);

	m = glm::translate(m, position);
	m = glm::scale(m, glm::vec3(scale));

	return m;
}