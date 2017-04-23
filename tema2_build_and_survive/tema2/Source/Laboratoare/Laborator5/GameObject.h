#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <string>

#include <Core/Engine.h>


class GameObject {

public:
	
	glm::mat4 translateMatrix;
	glm::mat4 rotateMatrix;
	glm::mat4 scaleMatrix;
	glm::mat4 rotateMatrixMap;
	glm::mat4 scaleMatrixMap;

	std::string nameScene;
	std::string nameMap;
	std::string shader;
	GameObject(std::string nameScene, std::string nameMap, std::string shader);

}; 