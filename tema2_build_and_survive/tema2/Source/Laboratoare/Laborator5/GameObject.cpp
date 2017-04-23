
#pragma once

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>

#include "GameObject.h"

GameObject::GameObject(std::string nameScene, std::string nameMap, std::string shader) : translateMatrix(glm::mat4(1)) , rotateMatrix(glm::mat4(1)), 
											  scaleMatrix(glm::mat4(1)), rotateMatrixMap(glm::mat4(1)), scaleMatrixMap(glm::mat4(1)),
											  nameScene(nameScene), nameMap(nameMap), shader(shader) { }
