#pragma once

#include <vector>
#include <Core/Engine.h>

using namespace std;

class Animation {

public:
	int totalFrames = 0;
	string name;
	vector<glm::vec2> moves;
	vector<glm::vec2> textures;
	vector<int> framesPerTexture;

	Animation();
	Animation(string name);
	~Animation();

};