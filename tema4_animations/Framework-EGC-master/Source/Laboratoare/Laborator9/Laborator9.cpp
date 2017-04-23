#include "Laborator9.h"

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>

using namespace std;

Laborator9::Laborator9()
{
}

Laborator9::~Laborator9()
{
}

void Laborator9::Init()
{
	const string textureLoc = "Source/Laboratoare/Laborator9/Textures/";
	sprite1.height = 3579;
	sprite1.width = 637;
	sprite1.nameTexture = "player1";

	sprite2.height = 3559;
	sprite2.width = 588;
	sprite2.nameTexture = "player2";

	getImagesPlayer2();
	getImagesPlayer1();

	player1 = new Player(glm::vec3(0, 0, 0), 3, 1);
	player2 = new Player(glm::vec3(0, 0, 0), 3, 1);

	createAnimationsPlayer1();
	createAnimationsPlayer2();

	readAnimations("animations1.txt", player1);
	readAnimations("animations2.txt", player2);
	// Load textures
	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "grass_bilboard.png").c_str(), GL_REPEAT);
		mapTextures["grass"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "crate.jpg").c_str(), GL_REPEAT);
		mapTextures["create"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "earth.png").c_str(), GL_REPEAT);
		mapTextures["earth"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "Player1.gif").c_str(), GL_REPEAT);
		mapTextures["player1"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "Player2.gif").c_str(), GL_REPEAT);
		mapTextures["player2"] = texture;
	}


	{
		Texture2D* texture = new Texture2D();
		texture->Load2D("Resources/Models/Vegetation/Bamboo/bamboo.png", GL_REPEAT);
		mapTextures["bamboo"] = texture;
	}

	{
		mapTextures["random"] = CreateRandomTexture(25, 25);
	}

	// Load meshes
	{
		Mesh* mesh = new Mesh("box");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("sphere");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("bamboo");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Vegetation/Bamboo/", "bamboo.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	// Create a simple quad
	{
		vector<glm::vec3> vertices
		{
			glm::vec3(0.5f,   0.5f, 0.0f),	// Top Right
			glm::vec3(0.5f,  -0.5f, 0.0f),	// Bottom Right
			glm::vec3(-0.5f, -0.5f, 0.0f),	// Bottom Left
			glm::vec3(-0.5f,  0.5f, 0.0f),	// Top Left
		};

		vector<glm::vec3> normals
		{
			glm::vec3(0, 1, 1),
			glm::vec3(1, 0, 1),
			glm::vec3(1, 0, 0),
			glm::vec3(0, 1, 0)
		};

		// TODO : Complete texture coords for the square
		vector<glm::vec2> textureCoords
		{
			glm::vec2(0.3f, 0.3f),
			glm::vec2(0.3f, 0.4f),
			glm::vec2(0.4f, 0.4f),
			glm::vec2(0.4f, 0.3f)
		};

		vector<unsigned short> indices =
		{
			0, 1, 3,
			1, 2, 3
		};

		Mesh* mesh = new Mesh("square");
		mesh->InitFromData(vertices, normals, textureCoords, indices);
		meshes[mesh->GetMeshID()] = mesh;
	}

	// Create a shader program for drawing face polygon with the color of the normal
	{
		Shader *shader = new Shader("ShaderLab9");
		shader->AddShader("Source/Laboratoare/Laborator9/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Laborator9/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}
}

void Laborator9::createAnimationsPlayer1() {


	ofstream fout("animations1.txt");

	//idle
	Animation idle("idle");
	idle.moves.push_back(glm::vec2(0, 0));
	idle.framesPerTexture.push_back(1);
	idle.textures.push_back(glm::vec2(1, 1));
	idle.totalFrames = 1;

	player1->animations.push_back(idle);

	Animation walking("walking");

	for (int i = 0; i < 6; ++i) {
		walking.moves.push_back(glm::vec2(0.05, 0));
		walking.framesPerTexture.push_back(4);
		walking.textures.push_back(glm::vec2(2, i));
		walking.totalFrames += 3;
	}

	player1->animations.push_back(walking);

	Animation hit("hit");

	for (int i = 0; i < 4; ++i) {
		hit.moves.push_back(glm::vec2(0.0f, 0));
		hit.framesPerTexture.push_back(4);
		hit.textures.push_back(glm::vec2(6, i));
		hit.totalFrames += 4;
	}

	player1->animations.push_back(hit);

	Animation stopHit("stophit");

	for (int i = 0; i < 3; ++i) {
		stopHit.moves.push_back(glm::vec2(0, 0));
		stopHit.framesPerTexture.push_back(4);
		stopHit.textures.push_back(glm::vec2(7, i));
		stopHit.totalFrames += 4;
	}

	player1->animations.push_back(stopHit);

	Animation stayHidden("hidden");

	stayHidden.moves.push_back(glm::vec2(0, 0));
	stayHidden.framesPerTexture.push_back(1);
	stayHidden.textures.push_back(glm::vec2(7, 2));
	stayHidden.totalFrames += 1;

	player1->animations.push_back(stayHidden);

	Animation fall("fall");
	for (int i = 0; i < 5; ++i) {
		fall.moves.push_back(glm::vec2(0, 0));
		fall.framesPerTexture.push_back(3);
		fall.textures.push_back(glm::vec2(15, 4 - i));
		fall.totalFrames += 3;
	}

	for (int i = 0; i < 3; ++i) {
		fall.moves.push_back(glm::vec2(0.0f, 0));
		fall.framesPerTexture.push_back(3);
		fall.totalFrames += 3;
		fall.textures.push_back(glm::vec2(16, i));
	}

	player1->animations.push_back(fall);

	Animation all("all");

	for (int i = 0; i < sprite1.images.size(); ++i)
		for (int j = 0; j < sprite1.images[i].size(); ++j) {
			all.moves.push_back(glm::vec2(0, 0));
			all.framesPerTexture.push_back(10);
			all.totalFrames += 10;
			all.textures.push_back(glm::vec2(i, j));
		}

	player1->animations.push_back(all);

	fout << 7 << '\n';

	for (Animation a : player1->animations) {

		fout << a.name << '\n';

		fout << a.moves.size() << '\n';

		for (int i = 0; i < a.moves.size(); ++i) {
			fout << a.moves[i].x << ' ' << a.moves[i].y << '\n';
			fout << a.framesPerTexture[i] << '\n';
			fout << a.textures[i].x << ' ' << a.textures[i].y << '\n';
		}
	}

	fout.close();

	player1->animations.clear();
}

void Laborator9::createAnimationsPlayer2() {

	ofstream fout("animations2.txt");

	Animation idle("idle");
	idle.moves.push_back(glm::vec2(0, 0));
	idle.framesPerTexture.push_back(1);
	idle.textures.push_back(glm::vec2(0, 0));
	idle.totalFrames = 1;

	player2->animations.push_back(idle);


	Animation walking("walking");

	for (int i = 0; i < 6; ++i) {

		walking.moves.push_back(glm::vec2(0.05f, 0));
		walking.framesPerTexture.push_back(4);
		walking.textures.push_back(glm::vec2(2, i));
		walking.totalFrames += 3;
	}


	for (int i = 0; i < 4; ++i) {

		walking.moves.push_back(glm::vec2(0.05f, 0));
		walking.framesPerTexture.push_back(3);
		walking.textures.push_back(glm::vec2(3, i));
		walking.totalFrames += 3;
	}

	player2->animations.push_back(walking);

	Animation hit("hit");

	for (int i = 3; i < 6; ++i) {
		hit.moves.push_back(glm::vec2(0.0f, 0));
		hit.framesPerTexture.push_back(6);
		hit.textures.push_back(glm::vec2(5, i));
		hit.totalFrames += 4;
	}


	hit.moves.push_back(glm::vec2(0.0f, 0));
	hit.framesPerTexture.push_back(4);
	hit.textures.push_back(glm::vec2(6, 0));
	hit.totalFrames += 4;

	player2->animations.push_back(hit);


	Animation stopHit("stophit");

	for (int i = 0; i < 3; ++i) {
		stopHit.moves.push_back(glm::vec2(0, 0));
		stopHit.framesPerTexture.push_back(4);
		stopHit.textures.push_back(glm::vec2(14, i));
		stopHit.totalFrames += 4;
	}

	player2->animations.push_back(stopHit);


	Animation stayHidden("hidden");

	stayHidden.moves.push_back(glm::vec2(0, 0));
	stayHidden.framesPerTexture.push_back(1);
	stayHidden.textures.push_back(glm::vec2(14, 2));
	stayHidden.totalFrames += 1;

	player2->animations.push_back(stayHidden);

	Animation fall("fall");

	
	for (int i = 1; i < 5; ++i) {
		fall.moves.push_back(glm::vec2(0.01f, 0));
		fall.framesPerTexture.push_back(3);
		fall.totalFrames += 3;
		fall.textures.push_back(glm::vec2(26, i));
	}

	for (int i = 0; i < 4; ++i) {
		fall.moves.push_back(glm::vec2(0.0f, 0));
		fall.framesPerTexture.push_back(3);
		fall.totalFrames += 3;
		fall.textures.push_back(glm::vec2(27, i));
	}

	player2->animations.push_back(fall);

	Animation all("all");

	for (int i = 0; i < sprite1.images.size(); ++i)
		for (int j = 0; j < sprite1.images[i].size(); ++j) {
			all.moves.push_back(glm::vec2(0, 0));
			all.framesPerTexture.push_back(10);
			all.totalFrames += 10;
			all.textures.push_back(glm::vec2(i, j));
		}

	player2->animations.push_back(all);
	
	fout << 7 << '\n';

	for (Animation a : player2->animations) {

		fout << a.name << '\n';

		fout << a.moves.size() << '\n';

		for (int i = 0; i < a.moves.size(); ++i) {
			fout << a.moves[i].x << ' ' << a.moves[i].y << '\n';
			fout << a.framesPerTexture[i] << '\n';
			fout << a.textures[i].x << ' ' << a.textures[i].y << '\n';
		}
	}

	player2->animations.clear();
	fout.close();
}

void Laborator9::readAnimations(string fileName, Player* player) {

	ifstream fin(fileName);
	int nrAnimations;

	fin >> nrAnimations;
	

	for (int i = 0; i < nrAnimations; ++i) {

		Animation a;

		fin >> a.name;
		int cnt;

		fin >> cnt;

		for (int i = 0; i < cnt; ++i) {

			glm::vec2 move;
			fin >> move.x >> move.y;
			int framesPerText;

			fin >> framesPerText;

			glm::vec2 text;
			fin >> text.x >> text.y;

			a.moves.push_back(move);
			a.framesPerTexture.push_back(framesPerText);
			a.textures.push_back(text);
			a.totalFrames += framesPerText;
		}

		player->animations.push_back(a);
	}
}

void Laborator9::drawSprite(float posX, float posY, int cx, int cy, Sprite s, Player* p) {

	float w = dimensionCharacters;
	float h = dimensionCharacters;
	vector<glm::vec3> vertices;

	if (p->orientation == 1) {

		vertices.push_back(glm::vec3(posX, posY + h, 0.0f));
		vertices.push_back(glm::vec3(posX + w, posY + h, 0.0f));
		vertices.push_back(glm::vec3(posX + w, posY, 0.0f));
		vertices.push_back(glm::vec3(posX, posY, 0.0f));
	}
	else {
		vertices.push_back(glm::vec3(posX + w, posY + h, 0.0f));
		vertices.push_back(glm::vec3(posX, posY + h, 0.0f));
		vertices.push_back(glm::vec3(posX, posY, 0.0f));
		vertices.push_back(glm::vec3(posX + w, posY, 0.0f));
	}

	int imgWidth = s.width;
	int imgHeight = s.height;
	
	int col1 = (int)s.images[cx][cy][0];
	int col2 = (int)s.images[cx][cy][2];
	int line1 = (int)s.images[cx][cy][1];
	int line2 = (int)s.images[cx][cy][3];


	const float tx1 = float(line1) / imgHeight;
	const float ty1 = float(col1) / imgWidth;

	const float tx2 = float(line2) / imgHeight;
	const float ty2 = float(col2) / imgWidth;

	/*
	vector<glm::vec2> textureCoords
	{
		glm::vec2(1, 0),
		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1)
	};

	*/

	vector<glm::vec2> textureCoords
	{
		glm::vec2(ty1, tx1),
		glm::vec2(ty2, tx1),
		glm::vec2(ty2, tx2),
		glm::vec2(ty1, tx2)
	};



	vector<glm::vec3> normals
	{
		glm::vec3(0, 1, 1),
		glm::vec3(1, 0, 1),
		glm::vec3(1, 0, 0),
		glm::vec3(0, 1, 0)
	};

	vector<unsigned short> indices =
	{
		0, 1, 3,
		1, 2, 3
	};

	// ... Bind the texture, enable the proper arrays

	Mesh* mesh = new Mesh("player");
	mesh->InitFromData(vertices, normals, textureCoords, indices);
	meshes[mesh->GetMeshID()] = mesh;

	RenderSimpleMesh(meshes["player"], shaders["ShaderLab9"], glm::mat4(1), 0, mapTextures[s.nameTexture], mapTextures["random"]);

};

void Laborator9::getImagesPlayer2() {

	vector<glm::vec4> v;

	//1
	v.push_back(glm::vec4(32, 3,91, 129));
	v.push_back(glm::vec4(129, 3, 168, 129));
	v.push_back(glm::vec4(168, 3, 236, 129));
	v.push_back(glm::vec4(236, 3, 414, 129));
	v.push_back(glm::vec4(311, 3, 523, 129));
	v.push_back(glm::vec4(396, 3, 629, 129));

	v.push_back(glm::vec4(474, 3, 629, 129));
	sprite2.images.push_back(v);
	v.clear();

	//2

	v.push_back(glm::vec4(23, 131, 120, 242));
	v.push_back(glm::vec4(129, 131, 236, 242));
	v.push_back(glm::vec4(236, 131, 343, 242));
	v.push_back(glm::vec4(446, 131, 561, 242));
	sprite2.images.push_back(v);
	v.clear();


	//3

	v.push_back(glm::vec4(5, 247, 103, 346));
	v.push_back(glm::vec4(103, 247, 207, 346));
	v.push_back(glm::vec4(207, 247, 312, 346));
	v.push_back(glm::vec4(312, 247, 414, 346));
	v.push_back(glm::vec4(414, 237, 503, 346));
	v.push_back(glm::vec4(503, 237, 582, 346));

	sprite2.images.push_back(v);
	v.clear();

	//4

	v.push_back(glm::vec4(16, 360, 89, 479));
	v.push_back(glm::vec4(89, 360, 170, 479));
	v.push_back(glm::vec4(170, 360, 249, 479));
	v.push_back(glm::vec4(249, 360, 341, 479));
	v.push_back(glm::vec4(341, 360, 416, 479));
	v.push_back(glm::vec4(416, 360, 491, 479));
	v.push_back(glm::vec4(491, 360, 561, 479));

	sprite2.images.push_back(v);
	v.clear();

	//5

	v.push_back(glm::vec4(10, 482, 74, 606));
	v.push_back(glm::vec4(74, 482, 151, 606));
	v.push_back(glm::vec4(151, 482, 229, 606));
	v.push_back(glm::vec4(229, 482, 306, 606));
	v.push_back(glm::vec4(306, 482, 422, 606));
	v.push_back(glm::vec4(422, 482, 491, 606));
	v.push_back(glm::vec4(491, 482, 581, 606));

	sprite2.images.push_back(v);
	v.clear();


	//6

	v.push_back(glm::vec4(10, 602, 93, 726));
	v.push_back(glm::vec4(93, 602, 177, 726));
	v.push_back(glm::vec4(177, 602, 252, 726));
	v.push_back(glm::vec4(252, 602, 345, 726));
	v.push_back(glm::vec4(345, 602, 437, 726));
	v.push_back(glm::vec4(437, 602, 562, 726));

	sprite2.images.push_back(v);
	v.clear();

	//7

	v.push_back(glm::vec4(43, 730, 164, 845));
	v.push_back(glm::vec4(164, 730, 266, 845));
	v.push_back(glm::vec4(266, 730, 376, 845));
	v.push_back(glm::vec4(376, 730, 544, 845));

	sprite2.images.push_back(v);
	v.clear();


	//8

	v.push_back(glm::vec4(18, 850, 274, 943));
	v.push_back(glm::vec4(274, 850, 439, 943));
	v.push_back(glm::vec4(439, 850, 551, 943));

	sprite2.images.push_back(v);
	v.clear();

	//9

	v.push_back(glm::vec4(34, 942, 155, 1051));
	v.push_back(glm::vec4(155, 942, 260, 1051));
	v.push_back(glm::vec4(260, 942, 364, 1051));

	v.push_back(glm::vec4(364, 942, 530, 1051));

	sprite2.images.push_back(v);
	v.clear();

	//10


	v.push_back(glm::vec4(2, 1047, 280, 1171));
	v.push_back(glm::vec4(280, 1047, 363, 1171));
	v.push_back(glm::vec4(363, 1047, 480, 1171));

	v.push_back(glm::vec4(480, 1047, 585, 1171));

	sprite2.images.push_back(v);
	v.clear();

	//11
	v.push_back(glm::vec4(2, 1296, 72, 1425));
	v.push_back(glm::vec4(72, 1296, 210, 1171));
	v.push_back(glm::vec4(210, 1296, 454, 1171));

	v.push_back(glm::vec4(454, 1296, 585, 1171));

	sprite2.images.push_back(v);
	v.clear();

	//12
	v.push_back(glm::vec4(2, 1423, 165, 1546));
	v.push_back(glm::vec4(165, 1423, 239, 1546));
	v.push_back(glm::vec4(239, 1423, 314, 1546));
	v.push_back(glm::vec4(314, 1423, 381, 1546));
	v.push_back(glm::vec4(381, 1423, 455, 1546));
	v.push_back(glm::vec4(455, 1423, 566, 1546));
	sprite2.images.push_back(v);
	v.clear();

	//13
	v.push_back(glm::vec4(2, 1542, 267, 1669));
	v.push_back(glm::vec4(267, 1542, 378, 1669));
	v.push_back(glm::vec4(378, 1542, 468, 1669));
	v.push_back(glm::vec4(468, 1542, 561, 1669));
	sprite2.images.push_back(v);
	v.clear();


	//14

	v.push_back(glm::vec4(2, 1669, 93, 1798));
	v.push_back(glm::vec4(93, 1669, 191, 1798));
	v.push_back(glm::vec4(191, 1669, 274, 1798));
	v.push_back(glm::vec4(274, 1669, 380, 1798));


	v.push_back(glm::vec4(380, 1669, 475, 1798));
	v.push_back(glm::vec4(475, 1669, 564, 1798));
	sprite2.images.push_back(v);
	v.clear();


	//15

	v.push_back(glm::vec4(2, 1800, 102, 1927));
	v.push_back(glm::vec4(102, 1800, 191, 1927));
	v.push_back(glm::vec4(191, 1800, 274, 1927));
	v.push_back(glm::vec4(274, 1800, 436, 1927));


	v.push_back(glm::vec4(436, 1800, 583, 1927));
	sprite2.images.push_back(v);
	v.clear();

	//16

	v.push_back(glm::vec4(2, 1928, 158, 2024));
	v.push_back(glm::vec4(158, 1928, 319, 2024));
	v.push_back(glm::vec4(319, 1928, 397, 2024));
	v.push_back(glm::vec4(397, 1928, 479, 2024));

	v.push_back(glm::vec4(479, 1928, 586, 2024));


	sprite2.images.push_back(v);
	v.clear();

	//17

	v.push_back(glm::vec4(2, 2015, 160, 2127));
	v.push_back(glm::vec4(158, 2015, 252, 2127));
	v.push_back(glm::vec4(252, 2015, 343, 2127));
	v.push_back(glm::vec4(343, 2015, 443, 2127));

	v.push_back(glm::vec4(443, 2015, 586, 2127));


	sprite2.images.push_back(v);
	v.clear();


	//18

	v.push_back(glm::vec4(2, 2129, 94, 2243));
	v.push_back(glm::vec4(94, 2129, 186, 2243));
	v.push_back(glm::vec4(186, 2129, 279, 2243));
	v.push_back(glm::vec4(279, 2129, 375, 2243));

	v.push_back(glm::vec4(375, 2129, 458, 2243));

	v.push_back(glm::vec4(458, 2129, 560, 2243));


	sprite2.images.push_back(v);
	v.clear();


	//19

	v.push_back(glm::vec4(2, 2246, 94, 2353));
	v.push_back(glm::vec4(94, 2246, 186, 2353));
	v.push_back(glm::vec4(186, 2246, 262, 2353));
	v.push_back(glm::vec4(262, 2246, 365, 2353));

	v.push_back(glm::vec4(365, 2246, 470, 2353));

	v.push_back(glm::vec4(470, 2246, 560, 2353));


	sprite2.images.push_back(v);
	v.clear();


	//20

	v.push_back(glm::vec4(2, 2353, 94, 2443));
	v.push_back(glm::vec4(94, 2353, 171, 2443));
	v.push_back(glm::vec4(171, 2353, 252, 2443));
	v.push_back(glm::vec4(252, 2353, 333, 2443));
	v.push_back(glm::vec4(333, 2353, 410, 2443));
	v.push_back(glm::vec4(410, 2353, 497, 2443));
	v.push_back(glm::vec4(497, 2353, 578, 2443));


	sprite2.images.push_back(v);
	v.clear();


	//21

	v.push_back(glm::vec4(2, 2439, 169, 2521));
	v.push_back(glm::vec4(169, 2439, 247, 2521));
	v.push_back(glm::vec4(247, 2439, 333, 2521));
	v.push_back(glm::vec4(333, 2439, 414, 2521));
	v.push_back(glm::vec4(414, 2439, 500, 2521));


	sprite2.images.push_back(v);
	v.clear();

	//22

	v.push_back(glm::vec4(2, 2534, 141, 2643));
	v.push_back(glm::vec4(141, 2534, 256, 2643));
	v.push_back(glm::vec4(256, 2534, 363, 2643));
	v.push_back(glm::vec4(363, 2534, 464, 2643));
	v.push_back(glm::vec4(464, 2534, 555, 2643));


	sprite2.images.push_back(v);
	v.clear();

	//23

	v.push_back(glm::vec4(2, 2664, 116, 2758));
	v.push_back(glm::vec4(116, 2664, 227, 2758));
	v.push_back(glm::vec4(227, 2664, 313, 2758));
	v.push_back(glm::vec4(313, 2664, 415, 2758));
	v.push_back(glm::vec4(415, 2664, 555, 2758));


	sprite2.images.push_back(v);
	v.clear();

	//24

	v.push_back(glm::vec4(2, 2763, 145, 2872));
	v.push_back(glm::vec4(145, 2763, 242, 2872));
	v.push_back(glm::vec4(242, 2763, 358, 2872));
	v.push_back(glm::vec4(358, 2763, 444, 2872));
	v.push_back(glm::vec4(444, 2763, 555, 2872));


	sprite2.images.push_back(v);
	v.clear();

	//25

	v.push_back(glm::vec4(2, 2870, 226, 3000));
	v.push_back(glm::vec4(226, 2870, 335, 3000));
	v.push_back(glm::vec4(335, 2870, 441, 3000));


	sprite2.images.push_back(v);
	v.clear();



	//26

	v.push_back(glm::vec4(2, 3002, 176, 3117));
	v.push_back(glm::vec4(176, 3002, 274, 3117));
	v.push_back(glm::vec4(274, 3002, 416, 3117));
	v.push_back(glm::vec4(416, 3002, 522, 3117));



	sprite2.images.push_back(v);
	v.clear();


	//27

	v.push_back(glm::vec4(2, 3125, 103, 3242));
	v.push_back(glm::vec4(103, 3125, 209, 3242));
	v.push_back(glm::vec4(209, 3125, 324, 3242));
	v.push_back(glm::vec4(324, 3125, 463, 3242));
	v.push_back(glm::vec4(463, 3125, 583, 3242));



	sprite2.images.push_back(v);
	v.clear();

	//28

	v.push_back(glm::vec4(2, 3240, 151, 3342));
	v.push_back(glm::vec4(151, 3240, 286, 3342));
	v.push_back(glm::vec4(286, 3240, 432, 3342));
	v.push_back(glm::vec4(432, 3240, 583, 3342));



	sprite2.images.push_back(v);
	v.clear();


	//29

	v.push_back(glm::vec4(2, 3341, 151, 3402));
	v.push_back(glm::vec4(151, 3341, 216, 3388));



	sprite2.images.push_back(v);
	v.clear();

}

void Laborator9::getImagesPlayer1() {

	vector<glm::vec4> v;

	//1
	v.push_back(glm::vec4(0, 0, 99, 116));
	v.push_back(glm::vec4(101, 0, 193, 116));
	v.push_back(glm::vec4(197, 0, 304, 116));
	v.push_back(glm::vec4(303, 0, 414, 116));
	v.push_back(glm::vec4(416, 0, 523, 116));
	v.push_back(glm::vec4(525, 0, 629, 116));
	sprite1.images.push_back(v);
	v.clear();

	//2

	v.push_back(glm::vec4(54, 129, 159, 240));
	v.push_back(glm::vec4(160, 129, 268, 240));
	v.push_back(glm::vec4(272, 129, 374, 240));
	v.push_back(glm::vec4(380, 129, 478, 240));
	v.push_back(glm::vec4(481, 129, 584, 240));

	sprite1.images.push_back(v);
	v.clear();

	//3

	v.push_back(glm::vec4(0, 258, 123, 359));
	v.push_back(glm::vec4(124, 258, 224, 359));
	v.push_back(glm::vec4(227, 258, 323, 359));
	v.push_back(glm::vec4(331, 258, 427, 359));
	v.push_back(glm::vec4(429, 258, 542, 359));
	v.push_back(glm::vec4(544, 258, 636, 359));


	sprite1.images.push_back(v);
	v.clear();


	//4
	v.push_back(glm::vec4(45, 374, 142, 479));
	v.push_back(glm::vec4(145, 374, 255, 479));
	v.push_back(glm::vec4(262, 374, 360, 479));
	v.push_back(glm::vec4(358, 374, 475, 479));
	v.push_back(glm::vec4(429, 374, 542, 479));
	v.push_back(glm::vec4(478, 374, 584, 479));


	sprite1.images.push_back(v);
	v.clear();

	//5

	v.push_back(glm::vec4(3, 486, 117, 598));
	v.push_back(glm::vec4(129, 486, 225, 598));
	v.push_back(glm::vec4(232, 486, 325, 598));
	v.push_back(glm::vec4(330, 486, 422, 598));
	v.push_back(glm::vec4(429, 486, 529, 598));
	v.push_back(glm::vec4(527, 486, 635, 598));


	sprite1.images.push_back(v);
	v.clear();

	//6
	v.push_back(glm::vec4(73, 609, 162, 754));
	v.push_back(glm::vec4(166, 609, 259, 754));
	v.push_back(glm::vec4(266, 609, 357, 754));
	v.push_back(glm::vec4(359, 609, 460, 754));
	v.push_back(glm::vec4(454, 609, 555, 769));


	sprite1.images.push_back(v);
	v.clear();

	//7
	v.push_back(glm::vec4(2, 782, 118, 909));
	v.push_back(glm::vec4(121, 782, 252, 909));
	v.push_back(glm::vec4(252, 782, 434, 909));
	v.push_back(glm::vec4(441, 782, 619, 909));


	sprite1.images.push_back(v);
	v.clear();

	//8

	v.push_back(glm::vec4(2, 921, 114, 1036));
	v.push_back(glm::vec4(122, 921, 284, 1036));
	v.push_back(glm::vec4(281, 921, 407, 1036));
	v.push_back(glm::vec4(412, 921, 510, 1036));

	v.push_back(glm::vec4(509, 921, 627, 1036));


	sprite1.images.push_back(v);
	v.clear();

	//9

	v.push_back(glm::vec4(2, 1048, 115, 1164));
	v.push_back(glm::vec4(162, 1048, 257, 1164));
	v.push_back(glm::vec4(270, 1048, 407, 1164));
	v.push_back(glm::vec4(410, 1048, 621, 1164));

	sprite1.images.push_back(v);
	v.clear();

	//10


	v.push_back(glm::vec4(2, 1167, 165, 1318));
	v.push_back(glm::vec4(164, 1167, 275, 1318));
	v.push_back(glm::vec4(275, 1167, 411, 1318));
	v.push_back(glm::vec4(410, 1167, 527, 1318));

	v.push_back(glm::vec4(527, 1167, 635, 1318));


	sprite1.images.push_back(v);
	v.clear();

	//11

	v.push_back(glm::vec4(2, 1327, 118, 1429));
	v.push_back(glm::vec4(118, 1327, 323, 1429));
	v.push_back(glm::vec4(322, 1327, 498, 1429));
	v.push_back(glm::vec4(498, 1327, 627, 1429));

	v.push_back(glm::vec4(627, 1327, 629, 1429));


	sprite1.images.push_back(v);
	v.clear();

	//12


	v.push_back(glm::vec4(2, 1439, 107, 1534));
	v.push_back(glm::vec4(107, 1439, 201, 1534));
	v.push_back(glm::vec4(201, 1439, 364, 1534));
	v.push_back(glm::vec4(364, 1439, 475, 1534));

	v.push_back(glm::vec4(475, 1439, 605, 1534));


	sprite1.images.push_back(v);
	v.clear();

	//13

	v.push_back(glm::vec4(2, 1531, 174, 1673));
	v.push_back(glm::vec4(174, 1531, 347, 1673));
	v.push_back(glm::vec4(347, 1531, 465, 1673));
	v.push_back(glm::vec4(465, 1531, 475, 1662));



	sprite1.images.push_back(v);
	v.clear();

	//14

	v.push_back(glm::vec4(2, 1678, 123, 1797));
	v.push_back(glm::vec4(123, 1678, 219, 1797));
	v.push_back(glm::vec4(219, 1678, 319, 1797));
	v.push_back(glm::vec4(319, 1678, 423, 1762));

	v.push_back(glm::vec4(423, 1661, 634, 1794));


	sprite1.images.push_back(v);
	v.clear();

	//15
	v.push_back(glm::vec4(2, 1822, 99, 1943));
	v.push_back(glm::vec4(99, 1822, 204, 1943));
	v.push_back(glm::vec4(205, 1822, 303, 1943));
	v.push_back(glm::vec4(303, 1822, 409, 1943));

	v.push_back(glm::vec4(415, 1820, 609, 1937));


	sprite1.images.push_back(v);
	v.clear();

	//16
	//last, falling


	v.push_back(glm::vec4(2, 2888, 130, 3003));
	v.push_back(glm::vec4(130, 2888, 287, 3003));
	v.push_back(glm::vec4(287, 2888, 398, 3003));
	v.push_back(glm::vec4(398, 2888, 510, 3003));

	v.push_back(glm::vec4(510, 2888, 636, 3003));


	sprite1.images.push_back(v);
	v.clear();

	//17

	v.push_back(glm::vec4(34, 3012, 200, 3124));
	v.push_back(glm::vec4(204, 3012, 390, 3105));
	v.push_back(glm::vec4(389, 3012, 579, 3090));

	sprite1.images.push_back(v);
	v.clear();

}

void Laborator9::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Laborator9::simpleRenderAnimation(Player* player, Sprite& sprite) {

	for (Animation a : player->animations) {

		int sum = 0;

		if (player->currentAnimation == a.name)
			for (int i = 0; i < a.framesPerTexture.size(); ++i) {

				sum += a.framesPerTexture[i];

				if (player->indexAnimation < sum) {
					drawSprite(player->pos.x, player->pos.y, a.textures[i].x, a.textures[i].y, sprite, player);
					break;
				}
				
			}
	}
}

void Laborator9::renderAnimation(Player* player, Sprite& sprite) {

	for (Animation a : player->animations)
		if (player->currentAnimation == a.name) {

			int sum = 0;

			for (int i = 0; i < a.framesPerTexture.size(); ++i) {
				
				sum += a.framesPerTexture[i];

				if (player->indexAnimation < sum) {

					player->pos.x += a.moves[i].x * player->orientation;
					player->pos.y += a.moves[i].y * player->orientation;
					drawSprite(player->pos.x, player->pos.y , a.textures[i].x, a.textures[i].y, sprite, player);

					if (((a.name == "stophit" || a.name == "hit" || a.name == "fall") && player->indexAnimation == a.totalFrames - 1) == false) {
						
						player->indexAnimation = player->indexAnimation + 1;
						
						if (player->indexAnimation == a.totalFrames) {
							player->cycles = 0;
							player->indexAnimation = 0;
						}
					}

					break;
				}
			}
		}

}

void Laborator9::Update(float deltaTimeSeconds)
{
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(1, 1, -3));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(2));
		RenderSimpleMesh(meshes["sphere"], shaders["ShaderLab9"], modelMatrix, 0, mapTextures["earth"]);
	}

	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(2, 0.5f, 0));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(60.0f), glm::vec3(1, 0, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.75f));
		RenderSimpleMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix, 0, mapTextures["create"]);
	}

	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-2, 0.5f, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.75f));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(75.0f), glm::vec3(1, 1, 0));
		RenderSimpleMesh(meshes["box"], shaders["ShaderLab9"], modelMatrix, 1, mapTextures["random"], mapTextures["grass"]);
	}

	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-2, 0.5f, -2));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(60.0f), glm::vec3(1, 0, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
		RenderSimpleMesh(meshes["cube"], shaders["ShaderLab9"], modelMatrix, 0, mapTextures["create"]);
	}

	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.5f, 0.0f));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
		RenderSimpleMesh(meshes["square"], shaders["ShaderLab9"], modelMatrix, 0, mapTextures["grass"]);
	}

	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-2, -0.5f, -3));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
		RenderSimpleMesh(meshes["bamboo"], shaders["ShaderLab9"], modelMatrix, 0, mapTextures["bamboo"]);
	}


	if (Engine::GetElapsedTime() - lastFrame > intervalFrames) {
		lastFrame = Engine::GetElapsedTime();
		frameIndex++;
	}

	frames++;

	if (frames % speed == 0) {
		if ((player1->pos.x - player2->pos.x) * (player1->pos.x - player2->pos.x) +
			(player1->pos.y - player2->pos.y) * (player1->pos.y - player2->pos.y) < dimensionCharacters * dimensionCharacters * 0.5f) {

			if (player1->currentAnimation == "hit" &&
				(player2->currentAnimation == "idle" || player2->currentAnimation == "hit" || player2->currentAnimation == "walking")
				&& ((player2->pos.x > player1->pos.x && player1->orientation == 1) ||
					(player2->pos.x < player1->pos.x && player1->orientation == -1))) {

				player2->currentAnimation = "fall";
				player2->cycles = 1;
				player2->indexAnimation = 0;
			}

			if (player2->currentAnimation == "hit" &&
				(player1->currentAnimation == "idle" || player1->currentAnimation == "hit" || player1->currentAnimation == "walking")
				&& ((player1->pos.x > player2->pos.x && player2->orientation == 1) ||
					(player1->pos.x < player2->pos.x && player2->orientation == -1))) {
				player1->currentAnimation = "fall";
				player1->indexAnimation = 0;
				player1->cycles = 1;
			}
		}
		renderAnimation(player1, sprite1);
		renderAnimation(player2, sprite2);
	} else {

		simpleRenderAnimation(player1, sprite1);
		simpleRenderAnimation(player2, sprite2);
	}


	/*
	drawSprite(0, 10, 0, 3, player1);
	drawSprite(0, 20, 0, 4, player1);
	drawSprite(0, 30, 0, 5, player1);

	drawSprite(10, 0, 1, 0, player1);
	drawSprite(10, 10, 1, 1, player1);
	drawSprite(10, 20, 1, 2, player1);
	drawSprite(10, 30, 1, 3, player1);

	drawSprite(10, 40, 1, 4, player1);
	*/
}

void Laborator9::FrameEnd()
{
	//DrawCoordinatSystem();
}

void Laborator9::RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix, int texture, Texture2D* texture1, Texture2D* texture2)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	// render an object using the specified shader and the specified position
	glUseProgram(shader->program);

	// Bind model matrix
	GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// Bind view matrix
	glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
	int loc_view_matrix = glGetUniformLocation(shader->program, "View");
	glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	// Bind projection matrix
	glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
	int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	int mix_location = glGetUniformLocation(shader->program, "mix_texture");
	glUniform1i(mix_location, texture);

	if (texture1)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1->GetTextureID());
		glUniform1i(glGetUniformLocation(shader->GetProgramID(), "texture_1"), 0);
	}

	if (texture2)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2->GetTextureID());
		glUniform1i(glGetUniformLocation(shader->GetProgramID(), "texture_2"), 1);
	}

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

Texture2D* Laborator9::CreateRandomTexture(unsigned int width, unsigned int height)
{
	GLuint textureID = 0;
	unsigned int channels = 3;
	unsigned int size = width * height * channels;
	unsigned char* data = new unsigned char[size];

	for (long i = 0; i < size; i = i + 3) {

		data[i] = rand() % 255;
	}
	unsigned int gl_texture_object;
	glGenTextures(1, &gl_texture_object);
	glBindTexture(GL_TEXTURE_2D, gl_texture_object);

	// TODO: generate random texture data
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);


	// Generate and bind the new texture ID
	glBindTexture(GL_TEXTURE_2D, textureID);

	// TODO: Set the texture parameters (MIN_FILTER, MAG_FILTER and WRAPPING MODE) using glTexParameteri

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	CheckOpenGLError();

	// TODO: Use glTextImage2D to set the texture data
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	// TODO: Generate texture mip-maps
	glGenerateMipmap(GL_TEXTURE_2D);

	CheckOpenGLError();

	// Save the texture into a wrapper Texture2D class for using easier later during rendering phase
	Texture2D* texture = new Texture2D();
	texture->Init(textureID, width, height, channels);

	SAFE_FREE_ARRAY(data);
	return texture;
}

// Documentation for the input functions can be found in: "/Source/Core/Window/InputController.h" or
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/Window/InputController.h

void Laborator9::OnInputUpdate(float deltaTime, int mods)
{
	float speed = 2;

	if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		glm::vec3 up = glm::vec3(0, 1, 0);
		glm::vec3 right = GetSceneCamera()->transform->GetLocalOXVector();
		glm::vec3 forward = GetSceneCamera()->transform->GetLocalOZVector();
		forward = glm::normalize(glm::vec3(forward.x, 0, forward.z));
	}

	if (window->KeyHold(GLFW_KEY_D) && !window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {


		if (player1->currentAnimation != "walking")
			player1->indexAnimation = 0;

		player1->currentAnimation = "walking";
		player1->orientation = 1;

	} else if(window->KeyHold(GLFW_KEY_A) && !window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {

		if (player1->currentAnimation != "walking")
			player1->indexAnimation = 0;

		player1->currentAnimation = "walking";
		player1->orientation = -1;

	} else if (window->KeyHold(GLFW_KEY_X) && !window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {

		if (player1->currentAnimation != "stophit")
			player1->indexAnimation = 0;

		player1->currentAnimation = "stophit";

	} else if (window->KeyHold(GLFW_KEY_Z) && !window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
		
		if (player1->currentAnimation != "hit")
			player1->indexAnimation = 0;

		player1->currentAnimation = "hit";

	} else if (window->KeyHold(GLFW_KEY_C) && !window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
		
		if (player1->currentAnimation != "fall")
			player1->indexAnimation = 0;

		player1->currentAnimation = "fall";

	} else if (window->KeyHold(GLFW_KEY_SPACE) && !window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
		
		if (player1->currentAnimation != "all")
			player1->indexAnimation = 0;

		player1->currentAnimation = "all";

	} else {

		if (player1->cycles == 0) {
			player1->currentAnimation = "idle";
			player1->indexAnimation = 0;
		}
	}

	if(computer == false) {


		if (window->KeyHold(GLFW_KEY_KP_4) && !window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
		
			if (player2->currentAnimation != "walking")
				player2->indexAnimation = 0;

			player2->currentAnimation = "walking";
			player2->orientation = -1;

		}
		else if (window->KeyHold(GLFW_KEY_KP_6) && !window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
		
			if (player2->currentAnimation != "walking")
				player2->indexAnimation = 0;

			player2->currentAnimation = "walking";
			player2->orientation = 1;
		} else if (window->KeyHold(GLFW_KEY_KP_3) && !window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {

			if (player2->currentAnimation != "stophit")
				player2->indexAnimation = 0;

			player2->currentAnimation = "stophit";

		} else if (window->KeyHold(GLFW_KEY_KP_1) && !window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {

			if (player2->currentAnimation != "hit")
				player2->indexAnimation = 0;

			player2->currentAnimation = "hit";
		}
		else if (window->KeyHold(GLFW_KEY_KP_7) && !window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
		
			if (player2->currentAnimation != "fall")
				player2->indexAnimation = 0;

			player2->currentAnimation = "fall";

		} else if (window->KeyHold(GLFW_KEY_SPACE) && !window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {

			if (player2->currentAnimation != "all")
				player2->indexAnimation = 0;

			player2->currentAnimation = "all";

		} else {

			if (player2->cycles == 0) {
				player2->currentAnimation = "idle";
				player2->indexAnimation = 0;
			}
		}
	}
	else {

		if (player1->currentAnimation == "attack" && abs(player1->pos.x - player2->pos.x) < 3) {

			if (player2->currentAnimation != "stophit") {
				player2->currentAnimation = "stophit";
				player2->indexAnimation = 0;
			}
			
		} else if (player1->currentAnimation == "idle" && fabs(player1->pos.x - player2->pos.x) < 2.5f) {
			
			if (player2->currentAnimation != "hit") {
				player2->currentAnimation = "hit";
				player2->indexAnimation = 0;
			}
		
		
		} else {

			if (player1->pos.x < player2->pos.x - 4) {
				player2->orientation = -1;

				if (player2->currentAnimation != "walking") {
					player2->currentAnimation = "walking";
					player2->indexAnimation = 0;
				}

			} else if (player1->pos.x > player2->pos.x + 4) {
	
				player2->orientation = 1;

				if (player2->currentAnimation != "walking") {
					player2->currentAnimation = "walking";
					player2->indexAnimation = 0;
				}
			}
			else {
				player2->currentAnimation = "idle";
				player2->indexAnimation = 0;
			}
		}
	}

}

void Laborator9::OnKeyPress(int key, int mods)
{
	if (key == GLFW_KEY_K) {
		player1->orientation *= -1;
		player2->orientation *= -1;
	}


	if (key == GLFW_KEY_O) {
		speed++;
	}

	if (key == GLFW_KEY_P)
		speed--;

	if (speed < 1)
		speed = 1;

	if (key == GLFW_KEY_V)
		computer = !computer;
}

void Laborator9::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Laborator9::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
}

void Laborator9::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Laborator9::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Laborator9::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Laborator9::OnWindowResize(int width, int height)
{
}
