#pragma once
#include <Component/SimpleScene.h>
#include "Camera.h"
#include "Enemy.h"
#include "Shuttle.h"
#include "GameObject.h"
#include "Tower.h"
#include "Bullet.h"
#include "Life.h"

class Game : public SimpleScene
{
	
	public:
		Game();
		~Game();

		void Init() override;

	private:
		void Update(float deltaTimeSeconds) override;
		void drawBigScene();
		void drawMiniMap();

		void actualizeShuttle(float deltaTime);
		void actualizeEnemies(float deltaTime);
		void actualizeBullets(float deltaTime);
		void actualizeLives(float deltaTime);
		void actualizeGrenades(float deltaTime);
		void actualizeRifles(float deltaTime);
		void actualizeTowers();

		void RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix, Laborator::Camera* camera);
		void drawObject(GameObject o, std::string where);
		void clearViewport();
		void FrameStart() override;

		void addEnemy(float& lastOccurence);
		void addTowers();
		void addBullet();
		void addLives();
		void addGrenade();
		void addRifle();
		
		void detectCollisions();
		void fireBullets(float& lastBulletFired);
		void addTowerBullet(glm::vec3 direction, Tower t);
		void setProjectionMatrix();

		//void Game::SetViewportArea(const ViewportSpace & viewSpace, glm::vec3 colorColor, bool clear);

		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
		void OnWindowResize(int width, int height) override;

	public:
		const int dimensionMinimap = 250;//dimensiunea viewport
		const int marginMinimap = 20; //margini intre viewport si minimap
		const float unitsMinimap = 9.0f;

		const float scaleEnemy = 0.4f;//dimensiune inamici
		const float scaleShutter = 0.08f;//dimensiune nava spatiala
		const float scaleBullet = 0.1f;
		const float scaleGrenade = 0.2f;
		const float scaleRifle = 0.07f;
		const float scaleObjMap = 0.6f;
		const float scaleTowers = 0.002f;
		const float scaleLife = 0.1f;

		const float turningPoint = 9.0f;
		
		const float distLifes = 0.45f;
		const float distTowers = 2.5f;

		const float distBullet = 10.0f;
		const float distRifle = 20.0f;
		const float distGrenade = 7.0f;

		const float velocityBullets = 4.2f;
		const float velocityGrenades = 2.6f;
		const float velocityRifles = 5.3f;
		const float velocityEnemies = 1.1f;
		const float velocityAnimation = 0.02f;
		const float velocityRotationTower = 0.5f;

		const float radiusCollision = 16.0f;
		
		const float periodBullets = 1.7f;
		const float periodEnemiesLow = 2.0f;
		const float periodBullets2 = 1.0f;
		const float periodGrenade = 3.0f;
		const float periodRifles = 0.3f;
		
		const float dyingTime = 1.5f;

		const float radiusTowers = 4.0f;

		float lastOccurence = 0;
		float lastBulletFired = 0;
		float periodEnemies = 5.0f;
		float lastGrenade = 0;
		float lastRifle = 0;
		float lastBullet = 0;

		Laborator::Camera *camera;
		Laborator::Camera *cameraAbove;

		std::vector<Enemy> enemies;
		std::vector<Tower> towers;
		std::vector<Bullet> bullets;
		std::vector<Bullet> grenades;
		std::vector<Bullet> rifles;
		std::vector<Life> lives;
		Shuttle shuttle;

		glm::mat4 projectionMatrix;
		glm::mat4 modelMatrix;
		bool renderCameraTarget;
};
