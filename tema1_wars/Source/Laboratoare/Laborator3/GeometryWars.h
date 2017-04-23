#pragma once

#include <Component/SimpleScene.h>
#include <string>
#include <Core/Engine.h>
#include "EnemyShip.h"
#include "MainShip.h"
#include "Bullet.h"
#include <set>
#include "Life.h"

class GeometryWars : public SimpleScene
{
	public:
		struct ViewportSpace
		{
			ViewportSpace() : x(0), y(0), width(1), height(1) {}
			ViewportSpace(int x, int y, int width, int height)
				: x(x), y(y), width(width), height(height) {}
			int x;
			int y;
			int width;
			int height;
		};

		struct LogicSpace
		{
			LogicSpace() : x(0), y(0), width(1), height(1) {}
			LogicSpace(float x, float y, float width, float height)
				: x(x), y(y), width(width), height(height) {}
			float x;
			float y;
			float width;
			float height;
		};

	public:
		GeometryWars();
		~GeometryWars();

		void Init() override;

	private:

		std::set<EnemyShip> enemies;
		std::set<Life> lives;
		MainShip ship;

		void drawScoreBoard(MainShip ship);
		void drawShip(Ship ship, std::string meshName, float dimScale);
		void actualizeEnemyShips(float deltaTime);
		void addEnemyShip();
		void moveShipAfterCursor();
		void testCollisions();
		void drawBullets();
		void actualizeBullets(float deltaTime);
		void addLife();
		void drawLife(std::string meshName1, std::string meshName2);
		void actualizeLives();

		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;

		void DrawScene(glm::mat3 visMatrix);

		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;

		// Sets the logic space and view space
		// logicSpace: { x, y, width, height }
		// viewSpace: { x, y, width, height }
		glm::mat3 VisualizationTransf2D(const LogicSpace &logicSpace, const ViewportSpace &viewSpace);
		glm::mat3 VisualizationTransf2DUnif(const LogicSpace &logicSpace, const ViewportSpace &viewSpace);

		void SetViewportArea(const ViewportSpace &viewSpace, glm::vec3 colorColor = glm::vec3(0), bool clear = true);
	public:
		static const int livesEnemies = 2;

	protected:


		bool freeze = false;
		
		float lastModification = 0;
		float enemyRadius;
		float collisionRadius;
		float bulletSize;
		float length;
		float absoluteTime = 0;
		float lastEnemyOccurrence = 0;
		float occurrenceInterval = 2.0;
		float freezingTime;
		
		const float intervalModifyTime = 10.0f;//how faster we change the rate of enemy sheeps appearence 
		const int livesMainShip = 3;
		const float shrinkFactor = 0.8f;
		const float speedUp = 1.3f;
		const float velocityEnemySheep = 0.3f;
		const float velocitySheep = 0.8f;
		const float velocityBullet = velocitySheep * 3.0f;
		const float multiplyEnemies = 0.9f;
		

		const glm::vec3 colorBackgroundInitial = glm::vec3(0);
		const glm::vec3 colorBullet = glm::vec3(1, 0, 0);
		const glm::vec3 colorMainShip = glm::vec3(1, 1, 0);
		const glm::vec3 colorEnemyShip1 = glm::vec3(1, 0, 1);
		const glm::vec3 colorEnemyShip2 = glm::vec3(0, 1, 1);
		const glm::vec3 colorSmallEnemyShip = glm::vec3(0, 0, 1);
		const glm::vec3 colorLife = glm::vec3(1, 1, 1);

		ViewportSpace viewSpace;
		LogicSpace logicSpace;
		glm::mat3 modelMatrix, visMatrix;
};
