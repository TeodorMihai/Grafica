#pragma once

#include <string>

#include <include/glm.h>
#include <Core/GPU/Mesh.h>

namespace Object2D
{

	// Create square with given bottom left corner, length and color
	Mesh* CreateSquare(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);

	//Create triangle centered in the centroid, length of side
	Mesh* CreateTriangle(std::string name, glm::vec3 centroid, float length, glm::vec3 color);

	//Create a bullet
	Mesh* CreateBullet(std::string name, glm::vec3 centroid, float length, glm::vec3 color);
	
	//create rectangle for number of lives
	Mesh* CreateRectangle(std::string name, glm::vec3 centroid, float length, glm::vec3 color);

	Mesh* CreateShip(std::string name, glm::vec3 centroid, float length, glm::vec3 color);

	Mesh* CreateLife(std::string name, glm::vec3 centroid, float length, glm::vec3 color);

	//Mesh* CreateInvin(std::string name, glm::vec3 centroid, float length, glm::vec3 color);
}

