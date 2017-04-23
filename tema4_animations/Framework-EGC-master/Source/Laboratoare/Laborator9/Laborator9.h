#pragma once
#include <Component/SimpleScene.h>
#include <Component/Transform/Transform.h>
#include <Core/GPU/Mesh.h>

#include "Player.h"
#include "Sprite.h"
#include "Animation.h"

class Laborator9 : public SimpleScene
{
public:
	Laborator9();
	~Laborator9();

	void Init() override;

private:
	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;

	void RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, int texture = 0, Texture2D* texture1 = NULL, Texture2D* texture2 = NULL);
	Texture2D* CreateRandomTexture(unsigned int width, unsigned int height);

	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;
	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
	void OnWindowResize(int width, int height) override;
	

	void drawSprite(float posX, float posY, int cx, int cy, Sprite s, Player* p);
	void createAnimationsPlayer1();
	void createAnimationsPlayer2();

	void readAnimations(string fileName, Player* player);

	void renderAnimation(Player* player, Sprite& sprite);
	void simpleRenderAnimation(Player* player, Sprite& sprite);

	void getImagesPlayer1();
	void getImagesPlayer2();

	Sprite sprite1;
	Sprite sprite2;
	float lastFrame = 0;
	float intervalFrames = 0.07f;


	const float dimensionCharacters = 2.0f;
	int frames = 0;
	int speed = 1;
	bool computer = false;

	float animationSpeed = 1.0f;

	Player* player1;
	Player* player2;


	int frameIndex = 0;
	std::unordered_map<std::string, Texture2D*> mapTextures;
	GLuint randomTextureID;
	int mix = 0;
};
