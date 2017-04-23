#pragma once

#include <Component/SimpleScene.h>
#include <Component/Transform/Transform.h>
#include <Core/GPU/Mesh.h>

#include "Sphere.h"
#include "Plane.h"
#include "Drop.h"
#include "Wave.h"
#include "Camera.h"

class Water : public SimpleScene
{
	public:
		Water();
		~Water();

		void Init() override;

	private:
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;

		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
		void OnWindowResize(int width, int height) override;
		

		void draw(GameObject o, glm::mat4 m, int type);
		void drawObjects();
		Mesh* CreatePlane(const char* name, glm::vec3 centroid, int units, int dimension, glm::vec3 color);
		Mesh* CreateMesh(const char* name, const std::vector<VertexFormat> &vertices, const std::vector<unsigned short> &indices);
		void RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, const glm::vec3 &color = glm::vec3(1));

		void releaseDrop();
		void actualizeDrops(float deltaTime);
		void actualizeWaves(float deltaTime);
		void changeWavesSpeed();

		float timeSpeed = 1.0f;

		Mycamera::Camera* camera;
		glm::mat4 projectionMatrix;


		Sphere s;
		Sphere light;
		Plane plane;
		std::vector<Drop> drops;
		std::vector<Wave> waves;

		float distPoints;

		const float scaleDrop = 0.1f;
		const float scaleLight = 0.5f;
		const float scaleSphere = 0.4f;

		const float gravityAcceleration = 9.8f;
		const float frictionCoefficient = 0.14f;
		const float waveLength = 0.18f;
		const float speed = 0.2f;
		const float amplitude = 0.1f;
		const float epsilon = 1e-5f;
		const int maxJumps = 6;
		const float waveLife = 20.0f;

		const float amplitudeFromMass = 0.45f;
		const float amplitudeFromVelocity = 0.003f;
		const float waveLengthFromMass = 0.7f;
		const float waveLengthFromVelocity = 0.02f;
		const float speedFromVelocity = 0.07f;
		const float amplitudeAttenuation = 0.006f;

		const unsigned int materialShininess = 30;
		const glm::vec3 lightDirection = glm::vec3(0, 1, 0);
		const float materialKd = 0.8f;
		const float materialKs = 0.8f;

		const float sensibiltyTime = 0.1f;

		const glm::vec3 colorDrops = glm::vec3(0, 0 , 139.0 / 255);
		const glm::vec3 colorPlane = glm::vec3(0.0f / 255, 0.0f / 255, 166.0f/ 255);
		const glm::vec3 colorSphere = glm::vec3(0, 0 , 139.0 / 255);
		const glm::vec3 colorLight = glm::vec3(1,1,1);
};
