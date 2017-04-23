#pragma once

#include <string>

using namespace std;

class Wave {

public:

	Wave::Wave(float amplitude = 0.1f, float waveLength = 0.18f, float speed = 0.2f,
				glm::vec3 pos = glm::vec3(0), float startTime = 0);
	~Wave();

	float amplitude;
	float waveLength;
	float speed;
	glm::vec3 pos;
	float startTime;
	float baseTimeM = 0;
	float baseTimeS = 0;
	float actualSpeed;

};