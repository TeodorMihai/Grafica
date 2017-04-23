#include "Game.h"

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>

#include "GameObject.h"

using namespace std;

Game::Game()
{
}

Game::~Game()
{
}

void Game::Init()
{
	camera = new Laborator::Camera();
	camera->Set(glm::vec3(0, 0, -camera->distanceToTarget), glm::vec3(0, 0, -camera->distanceToTarget + 1), glm::vec3(0, 1, 0));

	cameraAbove = new Laborator::Camera();
	cameraAbove->Set(glm::vec3(0, 1, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 1));

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
		Mesh* mesh = new Mesh("shuttle");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "shuttle.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("enemy");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "enemy.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("tower");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "tower.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	addLives();
	addTowers();
}

void Game::drawObject(GameObject o, std::string where) {

	if(where == "scene")
		RenderMesh(meshes[o.nameScene], shaders[o.shader], o.translateMatrix * o.rotateMatrix * o.scaleMatrix, camera);
	
	if(where == "minimap")
		RenderMesh(meshes[o.nameMap], shaders[o.shader], o.translateMatrix * o.rotateMatrixMap * o.scaleMatrixMap, cameraAbove);
}

void Game::drawBigScene() 
{

	if(shuttle.mode != Shuttle::RIFLE && shuttle.timeSinceDeath < 1.5f)
		drawObject(shuttle, "scene");

	if(shuttle.mode != Shuttle::RIFLE)
		for (Life life : lives)
			drawObject(life, "scene");

	for (Enemy e : enemies)
		drawObject(e, "scene");

	for (Tower t : towers)
		drawObject(t, "scene");

	for (Bullet b : bullets)
		drawObject(b, "scene");

	for (Bullet b : grenades)
		drawObject(b, "scene");

	for (Bullet b : rifles)
		drawObject(b, "scene");

	DrawCoordinatSystem(camera->GetViewMatrix(), projectionMatrix);
}

void Game::drawMiniMap()
{
	projectionMatrix = glm::ortho(-unitsMinimap, unitsMinimap, -unitsMinimap, unitsMinimap, -10.0f, 10.0f);

	DrawCoordinatSystem(cameraAbove->GetViewMatrix(), projectionMatrix);
	
	if(shuttle.timeSinceDeath < 1.5f)
		drawObject(shuttle, "minimap");

	for (Enemy e : enemies)
		drawObject(e, "minimap");

	for (Tower t : towers)
		drawObject(t, "minimap");
}

void Game::clearViewport() {

	glm::ivec2 resolution = window->GetResolution();
	glViewport(resolution.x - dimensionMinimap, 0, dimensionMinimap - marginMinimap, dimensionMinimap - marginMinimap);
	glEnable(GL_SCISSOR_TEST);
	glScissor(resolution.x - marginMinimap, 0, dimensionMinimap - marginMinimap, dimensionMinimap - marginMinimap);
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_SCISSOR_TEST);

}

void Game::FrameStart() {
	
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);

}

void Game::addEnemy(float& lastOccurence)	 {

	lastOccurence = (float)  Engine::GetElapsedTime();
	Enemy e;

	e.dimension = min(4.5f, max(1.0f, (float)Engine::GetElapsedTime() / 45.0f));
	e.translateMatrix = glm::translate(glm::mat4(1), glm::vec3(turningPoint, 0, turningPoint));
	e.rotateMatrix = glm::rotate(glm::mat4(1), RADIANS(-90), glm::vec3(0, 1, 0));
	e.scaleMatrix = glm::scale(glm::mat4(1) , glm::vec3(scaleEnemy));
	e.scaleMatrix = glm::scale(e.scaleMatrix, glm::vec3(e.dimension));


	e.rotateMatrixMap = glm::rotate(glm::mat4(1), RADIANS(90) , glm::vec3(0, 0, 1));;
	e.scaleMatrixMap = glm::scale(glm::mat4(1), glm::vec3(scaleObjMap));
	e.scaleMatrixMap = glm::scale(e.scaleMatrixMap, glm::vec3(e.dimension));

	e.lives = max(2, (int)Engine::GetElapsedTime() / 10);
	enemies.push_back(e);
}

void Game::addTowers() {

	for (int i = 0; i < 3; ++i) {

		Tower t;
		
		t.scaleMatrix = glm::scale(glm::mat4(1), glm::vec3(scaleTowers));
		t.scaleMatrixMap = glm::scale(glm::mat4(1), glm::vec3(scaleObjMap));

		if (i == 0)
			t.translateMatrix = glm::translate(glm::mat4(1), glm::vec3(-distTowers, 0, 0));
		else if (i == 1)
			t.translateMatrix = glm::translate(glm::mat4(1), glm::vec3(0, 0, distTowers));
		else if (i == 2)
			t.translateMatrix = glm::translate(glm::mat4(1), glm::vec3(distTowers, 0, 0));

		towers.push_back(t);
	}
}

void Game::addBullet() {

	Bullet b;
	b.translateMatrix = shuttle.translateMatrix;
	b.scaleMatrix = glm::scale(glm::mat4(1), glm::vec3(scaleBullet));
	float direction = 1.0f * atan2(camera->forward.z, camera->forward.x);
	b.direction3 = normalize(glm::vec3(cos(direction), 0, sin(direction)));
	b.power = 2;
	bullets.push_back(b);
}

void Game::addLives() {

	for (int i = 0; i < 3; ++i) {
		
		Life life;
		life.scaleMatrix = glm::scale(glm::mat4(1), glm::vec3(scaleLife));
		lives.push_back(life);
	}
}

void Game::addGrenade() {

	Bullet b;
	b.translateMatrix = shuttle.translateMatrix;
	b.scaleMatrix = glm::scale(glm::mat4(1), glm::vec3(scaleGrenade));
	float direction = 1.0f * atan2(camera->forward.z, camera->forward.x);
	b.direction3 = normalize(glm::vec3(cos(direction), 0, sin(direction)));
	b.power = 4;
	grenades.push_back(b);

}

void Game::addRifle() {

	Bullet b;
	b.translateMatrix = shuttle.translateMatrix;
	b.scaleMatrix = glm::scale(glm::mat4(1), glm::vec3(scaleRifle));
	float direction = 1.0f * atan2(camera->forward.z, camera->forward.x);
	b.direction3 = normalize(glm::vec3(cos(direction), 0, sin(direction)));
	b.power = 1;
	rifles.push_back(b);
}

void Game::actualizeShuttle(float deltaTime) {
	
	
	if (shuttle.lives <= 0 && shuttle.timeSinceDeath > dyingTime) shuttle.isAlive = false;
	
	if (shuttle.isAlive == false) return;

	if (shuttle.lives <= 0) {

		shuttle.timeSinceDeath += deltaTime;
		shuttle.rotateMatrix = glm::rotate(shuttle.rotateMatrix, shuttle.timeSinceDeath * velocityAnimation, glm::vec3(1, 1, 1));
		shuttle.scaleMatrix = glm::scale(shuttle.scaleMatrix, glm::vec3(0.997f));
		shuttle.rotateMatrixMap = glm::rotate(shuttle.rotateMatrixMap, shuttle.timeSinceDeath * velocityAnimation, glm::vec3(1, 1, 1));
	} else {
		shuttle.translateMatrix = glm::translate(glm::mat4(1), glm::vec3(camera->GetTargetPosition()));
		shuttle.rotateMatrix = glm::rotate(glm::mat4(1), RADIANS(-90.0f), glm::vec3(1.0f, 0, 0));
		shuttle.rotateMatrix = glm::rotate(shuttle.rotateMatrix, (float)-atan2(camera->forward.z, camera->forward.x) + (float)M_PI, glm::vec3(0, 0, 1.0f));
		shuttle.scaleMatrix = glm::scale(glm::mat4(1), glm::vec3(scaleShutter));

		shuttle.rotateMatrixMap = shuttle.rotateMatrix;
		shuttle.scaleMatrixMap = glm::scale(glm::mat4(1), glm::vec3(scaleObjMap));
	}
}

void Game::actualizeEnemies(float deltaTime) {

	if (Engine::GetElapsedTime() - lastOccurence >= periodEnemies) {
		addEnemy(lastOccurence);
		periodEnemies -= deltaTime;
		if (periodEnemies <= periodEnemiesLow)
			periodEnemies += deltaTime;
	}

	vector<Enemy> newEnemies;

	for (Enemy e : enemies) {

		Enemy newE = e;

		if (e.period == Enemy::FIRST)
			newE.translateMatrix = glm::translate(newE.translateMatrix, glm::vec3(-velocityEnemies * deltaTime, 0, 0));
		
		if (e.period == Enemy::SECOND)
			newE.translateMatrix = glm::translate(newE.translateMatrix, glm::vec3(velocityEnemies * deltaTime * cos(RADIANS(45)) , 0, -velocityEnemies * deltaTime * sin(RADIANS(45)) ));

		if (e.period == Enemy::THIRD)
			newE.translateMatrix = glm::translate(newE.translateMatrix, glm::vec3(-velocityEnemies * deltaTime, 0, 0));

		if (e.period == Enemy::FOURTH) {

			newE.timeSinceDeath += deltaTime;

			newE.rotateMatrix = glm::rotate(newE.rotateMatrix, newE.timeSinceDeath * velocityAnimation, glm::vec3(1, 1, 1));
			newE.scaleMatrix = glm::scale(newE.scaleMatrix, glm::vec3(0.997f));
		}

		glm::vec3 centroid = glm::vec3(newE.translateMatrix * newE.rotateMatrix * newE.scaleMatrix * glm::vec4(0, 0, 0, 1));

		if (newE.period == Enemy::THIRD && centroid.x < -turningPoint) {

			shuttle.lives--;

			if(lives.empty() == false)
				lives.pop_back();
			continue;
		}

		if (newE.period == Enemy::SECOND && centroid.x > turningPoint) {
			newE.period = e.THIRD;
			newE.rotateMatrix = glm::rotate(newE.rotateMatrix, RADIANS(135), glm::vec3(0, 1, 0));
			newE.rotateMatrixMap = glm::rotate(glm::mat4(1), RADIANS(135), glm::vec3(0, 1, 0)) * newE.rotateMatrixMap;
		} 

		if (newE.period == Enemy::FIRST && centroid.x < -turningPoint) {
			newE.period = e.SECOND;
			newE.rotateMatrix = glm::rotate(newE.rotateMatrix, RADIANS(-135), glm::vec3(0, 1, 0));
			newE.rotateMatrixMap = glm::rotate(glm::mat4(1), RADIANS(-135), glm::vec3(0, 1, 0)) * newE.rotateMatrixMap;
		}

		newEnemies.push_back(newE);
	}
	
	enemies = newEnemies;

	for (Enemy& e : enemies)
		if (e.lives <= 0 && e.timeSinceDeath < dyingTime)
			e.rotateMatrixMap = glm::rotate(e.rotateMatrixMap, e.timeSinceDeath * velocityAnimation, glm::vec3(1, 1, 1));
}

void Game::actualizeBullets(float deltaTime) {

	if (Engine::GetElapsedTime() - lastBulletFired >= periodBullets)
		fireBullets(lastBulletFired);

	std::vector<Bullet> newBullets;

	for (Bullet& b : bullets) {

		b.timeSinceLaunch += deltaTime;
		b.translateMatrix = glm::translate(b.translateMatrix, b.direction3 * velocityBullets * deltaTime);
		b.distance += glm::distance(glm::vec3(0) , b.direction3 * velocityBullets * deltaTime);
	}

	for (Bullet b : bullets) {

		if (b.distance > distBullet) continue;
		
		newBullets.push_back(b);
	}

	bullets = newBullets;
}

void Game::actualizeGrenades(float deltaTime) {

	std::vector<Bullet> newGrenades;

	for (Bullet& b : grenades) {

	
		b.translateMatrix = glm::translate(b.translateMatrix, b.direction3 * velocityGrenades * deltaTime);

		if (b.distance < distGrenade * 0.5) {
			b.timeSinceLaunch += deltaTime;
			b.translateMatrix = glm::translate(b.translateMatrix, glm::vec3(0, 0.1 / b.timeSinceLaunch, 0) * deltaTime);
		}
		else {
			b.timeSinceLaunch -= deltaTime;
			b.translateMatrix = glm::translate(b.translateMatrix, glm::vec3(0, -0.1 / b.timeSinceLaunch, 0) * deltaTime);
		}

		b.distance += glm::distance(glm::vec3(0), b.direction3 * velocityGrenades * deltaTime);
	}

	for (Bullet b : grenades) {

		if (b.distance > distGrenade) continue;

		newGrenades.push_back(b);
	}

	grenades = newGrenades;
}

void Game::actualizeRifles(float deltaTime) {

	std::vector<Bullet> newRifles;

	for (Bullet& b : rifles) {

		b.timeSinceLaunch += deltaTime;
		b.translateMatrix = glm::translate(b.translateMatrix, b.direction3 * velocityRifles * deltaTime);
		b.distance += glm::distance(glm::vec3(0), b.direction3 * velocityRifles * deltaTime);
	}


	for (Bullet b : rifles) {

		if (b.distance > distRifle) continue;

		newRifles.push_back(b);
	}

	rifles = newRifles;
}

void Game::actualizeLives(float deltaTime) {

	//ox, oy, oz
	for (unsigned i = 0; i < lives.size(); ++i) {

		if (i == 0) {

			lives[i].translateMatrix = shuttle.translateMatrix;
			lives[i].translateMatrix *= glm::rotate(glm::mat4(1), (float)Engine::GetElapsedTime(), glm::vec3(1, 0, 0));
			lives[i].translateMatrix *= glm::translate(glm::mat4(1), glm::vec3(0, 0, distLifes));

		} else if (i == 1) {

			lives[i].translateMatrix = shuttle.translateMatrix;
			lives[i].translateMatrix *= glm::rotate(glm::mat4(1), (float)Engine::GetElapsedTime(), glm::vec3(0, 1, 0));
			lives[i].translateMatrix *= glm::translate(glm::mat4(1), glm::vec3(0, 0, distLifes));
		} else if (i == 2) {
			
			lives[i].translateMatrix = shuttle.translateMatrix;
			lives[i].translateMatrix *= glm::rotate(glm::mat4(1), (float)Engine::GetElapsedTime(), glm::vec3(0, 0, 1));
			lives[i].translateMatrix *= glm::translate(glm::mat4(1), glm::vec3(distLifes, 0, 0));
			
		}
	}

}

void Game::actualizeTowers() {

	for (Tower& t : towers)
		t.rotateMatrix = glm::rotate(glm::mat4(1), (float) Engine::GetElapsedTime(), glm::vec3(0, 1, 0));
}

void Game::detectCollisions() {

	std::vector<Enemy> newEnemies;
	std::vector<Bullet> newBullets;
	std::vector<Bullet> newGrenades;
	std::vector<Bullet> newRifles;

	for (Enemy& e : enemies) {

		bool enemyAlive = true;
		
		glm::vec3 centroidEnemy = glm::vec3(e.translateMatrix * e.rotateMatrix * e.scaleMatrix * glm::vec4(0, 0, 0, 1));

		for (Bullet b : bullets) {

			bool bulletAlive = true;

			glm::vec3 centroidBullet = glm::vec3(b.translateMatrix * b.rotateMatrix * b.scaleMatrix * glm::vec4(0, 0, 0, 1));

			if (glm::distance(centroidBullet, centroidEnemy) <= scaleEnemy * scaleBullet * radiusCollision * e.dimension && e.lives > 0) {
				
				bulletAlive = false;
				e.lives =  max(0, e.lives - b.power);

				if (e.lives == 0)
					e.period = Enemy::FOURTH;
			}

			if (bulletAlive == true)
				newBullets.push_back(b);
		}

		bullets = newBullets;
		newBullets.clear();

		for (Bullet b : grenades) {

			bool bulletAlive = true;

			glm::vec3 centroidBullet = glm::vec3(b.translateMatrix * b.rotateMatrix * b.scaleMatrix * glm::vec4(0, 0, 0, 1));

			if (glm::distance(centroidBullet, centroidEnemy) <= scaleEnemy * scaleBullet * radiusCollision * e.dimension && e.lives > 0) {

				bulletAlive = false;
				e.lives = max(0, e.lives - b.power);

				if (e.lives == 0)
					e.period = Enemy::FOURTH;
			}

			if (bulletAlive == true)
				newGrenades.push_back(b);
		}

		grenades = newGrenades;
		newGrenades.clear();


		for (Bullet b : rifles) {

			bool bulletAlive = true;

			glm::vec3 centroidBullet = glm::vec3(b.translateMatrix * b.rotateMatrix * b.scaleMatrix * glm::vec4(0, 0, 0, 1));

			if (glm::distance(centroidBullet, centroidEnemy) <= scaleEnemy * scaleBullet * radiusCollision * e.dimension && e.lives > 0) {

				bulletAlive = false;
				e.lives = max(0, e.lives - b.power);

				if (e.lives == 0)
					e.period = Enemy::FOURTH;
			}

			if (bulletAlive == true)
				newRifles.push_back(b);
		}

		if (enemyAlive == true && e.timeSinceDeath < 1.5f)
			newEnemies.push_back(e);

		rifles = newRifles;
		newRifles.clear();
	}

	enemies = newEnemies;
}

void Game::addTowerBullet(glm::vec3 direction, Tower t) {

	Bullet b;
	b.direction3 = direction;
	b.scaleMatrix = glm::scale(glm::mat4(1), glm::vec3(scaleBullet));
	b.translateMatrix = t.translateMatrix;
	bullets.push_back(b);
}

void Game::fireBullets(float& lastBulletFired) {

	lastBulletFired = (float) Engine::GetElapsedTime();

	for (Tower t : towers) {

		glm::vec3 centroidTower = glm::vec3(t.translateMatrix * glm::vec4(0, 0, 0, 1));

		float closestEnemy = radiusTowers;
		bool findCloseEnemy = false;
		glm::vec3 fireDirection(0);

		for (Enemy e : enemies) {

			glm::vec3 centroidEnemy = glm::vec3(e.translateMatrix * glm::vec4(0, 0, 0, 1));
			
			if (glm::distance(centroidEnemy, centroidTower) < closestEnemy) {
				closestEnemy = glm::distance(centroidEnemy, centroidTower);
				fireDirection = normalize(centroidEnemy - centroidTower);
				fireDirection *= 0.6f;
				findCloseEnemy = true;
			}
		}

		if (findCloseEnemy == true) 
			addTowerBullet(fireDirection, t);
	}
}

void Game::setProjectionMatrix() {

	if(shuttle.mode == Shuttle::PISTOL)
		projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);

	if(shuttle.mode == Shuttle::RIFLE)
		projectionMatrix = glm::perspective(RADIANS(20), window->props.aspectRatio, 0.01f, 200.0f);

	if(shuttle.mode == Shuttle::LAUNCHER)
		projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);
}

void Game::Update(float deltaTimeSeconds)
{
	
	FrameStart();

	actualizeShuttle(deltaTimeSeconds);
	actualizeEnemies(deltaTimeSeconds);
	actualizeBullets(deltaTimeSeconds);
	actualizeLives(deltaTimeSeconds);
	actualizeGrenades(deltaTimeSeconds);
	actualizeRifles(deltaTimeSeconds);
	actualizeTowers();
	detectCollisions();

	drawBigScene();
	
	clearViewport();
	
	drawMiniMap();

	setProjectionMatrix();
}

void Game::RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix, Laborator::Camera* camera)
{
	if (!mesh || !shader || !shader->program)
		return;

	// render an object using the specified shader and the specified position
	shader->Use();
	glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
	glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	mesh->Render();
}

// Documentation for the input functions can be found in: "/Source/Core/Window/InputController.h" or
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/Window/InputController.h

void Game::OnInputUpdate(float deltaTime, int mods)
{
	// move the camera only if MOUSE_RIGHT button is pressed
	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		float cameraSpeed = 2.0f;

		if (window->KeyHold(GLFW_KEY_W)) {
			glm::vec3 translated = camera->TranslateForward(cameraSpeed * deltaTime);
			cameraAbove->translate(translated);
		}

		if (window->KeyHold(GLFW_KEY_A)) {
			glm::vec3 translated = camera->TranslateRight(-cameraSpeed * deltaTime);
			cameraAbove->translate(translated);
		}

		if (window->KeyHold(GLFW_KEY_S)) {
			glm::vec3 translated = camera->TranslateForward(-cameraSpeed * deltaTime);
			cameraAbove->translate(translated);
		}

		if (window->KeyHold(GLFW_KEY_D)) {
			glm::vec3 translated = camera->TranslateRight(cameraSpeed * deltaTime);
			cameraAbove->translate(translated);
		}

		if (window->KeyHold(GLFW_KEY_Q)) {
			camera->TranslateUpword(cameraSpeed * deltaTime);
		}

		if (window->KeyHold(GLFW_KEY_E)) {
			camera->TranslateUpword(-cameraSpeed * deltaTime);
		}
	}
}

void Game::OnKeyPress(int key, int mods)
{
	if (key == GLFW_KEY_1)
		shuttle.mode = Shuttle::PISTOL;

	if (key == GLFW_KEY_2) 
		shuttle.mode = Shuttle::RIFLE;

	if (key == GLFW_KEY_3) 
		shuttle.mode = Shuttle::LAUNCHER;
}

void Game::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Game::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event

	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		float sensivityOX = 0.001f;
		float sensivityOY = 0.001f;

		camera->RotateThirdPerson_OX(-deltaY * sensivityOY);
		camera->RotateThirdPerson_OY(-deltaX * sensivityOX);
	}
}

void Game::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press 
	if (button == GLFW_MOUSE_BUTTON_2 && shuttle.isAlive == true) {
		
		if (shuttle.mode == Shuttle::PISTOL && Engine::GetElapsedTime() - lastBullet >= periodBullets2) {
			addBullet();
			lastBullet = (float) Engine::GetElapsedTime();
		}

		if (shuttle.mode == Shuttle::LAUNCHER && Engine::GetElapsedTime() - lastGrenade >= periodGrenade) {
			addGrenade();
			lastGrenade = (float) Engine::GetElapsedTime();
		}

		if (shuttle.mode == Shuttle::RIFLE && Engine::GetElapsedTime() - lastRifle >= periodRifles) {
			addRifle();
			lastRifle = (float) Engine::GetElapsedTime();
		}
	}
}

void Game::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Game::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Game::OnWindowResize(int width, int height)
{
}
