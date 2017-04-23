#pragma once

#include <vector>
#include <Core/Engine.h>

using namespace std;

class Sprite {

public:
	int width;
	int height;
	string nameTexture;
	vector< vector<glm::vec4> > images;
	Sprite();
	Sprite(int width, int height);
	~Sprite();

};