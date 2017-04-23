#include "GameObject.h"
#include "Wave.h"

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>

using namespace std;

Wave::Wave(float amplitude, float waveLength, float speed, glm::vec3 pos, float startTime) :
	amplitude(amplitude), waveLength(waveLength), speed(speed), pos(pos), startTime(startTime) ,
	actualSpeed(speed) { }

Wave::~Wave() { }