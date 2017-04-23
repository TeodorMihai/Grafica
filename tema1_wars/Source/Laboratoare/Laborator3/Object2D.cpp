#include "Object2D.h"

#include <Core/Engine.h>
#include "Transform2D.h"

Mesh* Object2D::CreateSquare(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill)
{
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(length, 0, 0), color),
		VertexFormat(corner + glm::vec3(length, length, 0), color),
		VertexFormat(corner + glm::vec3(0, length, 0), color)
	};

	Mesh* square = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3 };
	
	if (!fill) {
		square->SetDrawMode(GL_LINE_LOOP);
	}
	else {
		// draw 2 triangles. Add the remaining 2 indices
		indices.push_back(0);
		indices.push_back(2);
	}

	square->InitFromData(vertices, indices);
	return square;
}

Mesh* Object2D::CreateTriangle(std::string name, glm::vec3 centroid, float length, glm::vec3 color)
{
	float height = sqrt(3.0f) * length / 2.0f;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(centroid + glm::vec3(0, height * 2.0 / 3.0, 0), color),
		VertexFormat(centroid + Transform2D::Rotate( (float) M_PI * 2.0 / 3.0) * glm::vec3(0, height * 2.0 / 3.0, 0) , color),
		VertexFormat(centroid + Transform2D::Rotate( (float) M_PI * 4.0 / 3.0) * glm::vec3(0, height * 2.0 / 3.0, 0) , color)
	};

	Mesh* square = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2 };

	square->InitFromData(vertices, indices);
	return square;
}

Mesh* Object2D::CreateShip(std::string name, glm::vec3 centroid, float length, glm::vec3 color)
{
	float height = sqrt(3.0f) * length / 2.0f;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(centroid + glm::vec3(-length / 2, -height, 0), color),
		VertexFormat(centroid + glm::vec3(length / 2, -height, 0) , color),
		VertexFormat(centroid , color),
		VertexFormat(centroid + glm::vec3(-length / 2, height, 0) , color),
		VertexFormat(centroid + glm::vec3(length / 2, height, 0) , color)

	};

	Mesh* ship = new Mesh(name);
	std::vector<unsigned short> indices = { 
		
		0, 1, 2,
		2, 3, 4,
	
	};

	ship->InitFromData(vertices, indices);
	return ship;
}


Mesh* Object2D::CreateBullet(std::string name, glm::vec3 centroid, float bulletSize, glm::vec3 color)
{
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(centroid + glm::vec3(-bulletSize * 0.5f, -bulletSize * 0.5f , 0), color),
		VertexFormat(centroid + glm::vec3(-bulletSize * 0.5f, bulletSize * 0.5f , 0) , color),
		VertexFormat(centroid + glm::vec3(bulletSize * 0.5f, bulletSize * 0.5, 0), color),
		VertexFormat(centroid + glm::vec3(bulletSize * 0.5f, -0.5f * bulletSize, 0) , color),

	};

	Mesh* bullet = new Mesh(name);
	std::vector<unsigned short> indices = {
		0, 1, 2,
		2, 3, 0,
	};

	bullet->InitFromData(vertices, indices);
	return bullet;
}

Mesh* Object2D::CreateRectangle(std::string name, glm::vec3 centroid, float length, glm::vec3 color)
{

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(centroid + glm::vec3(-length * 0.5f, -length * 4, 0), color),
		VertexFormat(centroid + glm::vec3(-length * 0.5f, length * 4, 0) , color),
		VertexFormat(centroid + glm::vec3(length * 0.5f, 4 * length, 0), color),
		VertexFormat(centroid + glm::vec3(length * 0.5f, -4 * length, 0) , color),

	};

	Mesh* rectangle = new Mesh(name);
	std::vector<unsigned short> indices = { 
		0, 1, 2,
		2, 3, 0
	};

	rectangle->InitFromData(vertices, indices);
	return rectangle;
}

Mesh* Object2D::CreateLife(std::string name, glm::vec3 centroid, float length, glm::vec3 color)
{
	std::vector<VertexFormat> vertices;

	vertices.push_back(VertexFormat(centroid, color));
	int nrTriangles = 500;

	for (int i = 0; i <= nrTriangles; ++i)
		vertices.push_back(VertexFormat(centroid + glm::vec3(length * cos(2.0 * (float)M_PI * i / nrTriangles),
			length * sin(2.0 * (float)M_PI * i / nrTriangles), 0), color));
	
	std::vector<unsigned short> indices;

	Mesh* circle = new Mesh(name);
	
	for (unsigned short i = 0; i <= nrTriangles; ++i)
		indices.push_back(i);

	circle->SetDrawMode(GL_POINTS);
	circle->InitFromData(vertices, indices);
	
	return circle;
}

/*
Mesh* CreateInvin(std::string name, glm::vec3 centroid, float length, glm::vec3 color) {


	std::vector<VertexFormat> vertices;

	vertices.push_back(VertexFormat(centroid, color));
	int nrTriangles = 500;

	for (int i = 0; i <= nrTriangles; ++i)
		vertices.push_back(VertexFormat(glm));

}
*/