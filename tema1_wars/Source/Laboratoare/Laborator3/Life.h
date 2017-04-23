#include <Component/SimpleScene.h>
#include <string>
#include <Core/Engine.h>

class Life
{

public:
	glm::vec3 centroid;
	float size;
	Life();
	Life(glm::vec3 _centroid, float _size);
	~Life();

	bool operator<(const Life& b) const;
};