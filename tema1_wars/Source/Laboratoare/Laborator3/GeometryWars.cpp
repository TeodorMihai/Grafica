#include "GeometryWars.h"

#include <vector>
#include <iostream>
#include <set>

#include <Core/Engine.h>
#include "Transform2D.h"
#include "Object2D.h"
#include "Ship.h"
#include "MainShip.h"
#include "Bullet.h"

using namespace std;

GeometryWars::GeometryWars()
{
}

GeometryWars::~GeometryWars()
{
}

void GeometryWars::Init()
{
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	cameraInput->SetActive(false);

	logicSpace.x = -1;		// logic x
	logicSpace.y = -1;		// logic y
	logicSpace.width = 2;	// logic width
	logicSpace.height = 2;	// logic height

	glm::vec3 center = glm::vec3(logicSpace.x + logicSpace.width / 2.0 , logicSpace.y + logicSpace.height / 2.0 , 1.0);
	length = min(logicSpace.width, logicSpace.height) / 20.0f;
	enemyRadius = min(logicSpace.width, logicSpace.height) / 2.0f;
	collisionRadius = length / 1.7f;
	bulletSize = length / 7.0f;

	Mesh* mainShip = Object2D::CreateShip("mainShip", center, length, colorMainShip);
	Mesh* enemyShip1 = Object2D::CreateShip("enemyShip1", center, length, colorEnemyShip1);
	Mesh* enemyShip2 = Object2D::CreateShip("enemyShip2", center, length, colorSmallEnemyShip);
	Mesh* enemyShip3 = Object2D::CreateShip("enemyShip3", center, length, colorEnemyShip2);
	Mesh* bullet1 = Object2D::CreateBullet("bullet1", center, bulletSize, colorBullet);
	Mesh* rectangle1 = Object2D::CreateRectangle("rectangle1", center,length / 5.0f , colorBullet);
	Mesh* life1 = Object2D::CreateLife("life1", center, length / 2.0f, colorLife);
	Mesh* life2 = Object2D::CreateLife("life2", center, length / 3.0f, colorLife);

	//Mesh* invin1 = Object2D::CreateLife("invin1", center, length / 2.0f, colorLife);
	//Mesh* invin2 = Object2D::CreateLife("invin2", center, length / 3.0f, colorLife);

	
	ship.centroid = center;
	ship.sideLength = length;
	ship.orientation = 0;
	ship.velocity = velocitySheep;
	ship.lives = livesMainShip;

	AddMeshToList(mainShip);
	AddMeshToList(enemyShip1);
	AddMeshToList(enemyShip2);
	AddMeshToList(enemyShip3);
	AddMeshToList(bullet1);
	AddMeshToList(rectangle1);
	AddMeshToList(life1);
	AddMeshToList(life2);
}

// 2D visualization matrix
glm::mat3 GeometryWars::VisualizationTransf2D(const LogicSpace & logicSpace, const ViewportSpace & viewSpace)
{
	float sx, sy, tx, ty;
	sx = viewSpace.width / logicSpace.width;
	sy = viewSpace.height / logicSpace.height;
	tx = viewSpace.x - sx * logicSpace.x;
	ty = viewSpace.y - sy * logicSpace.y;

	return glm::transpose(glm::mat3(
		sx, 0.0f, tx,
		0.0f, sy, ty,
		0.0f, 0.0f, 1.0f));
}

// uniform 2D visualization matrix (same scale factor on x and y axes)
glm::mat3 GeometryWars::VisualizationTransf2DUnif(const LogicSpace & logicSpace, const ViewportSpace & viewSpace)
{
	float sx, sy, tx, ty, smin;
	sx = viewSpace.width / logicSpace.width;
	sy = viewSpace.height / logicSpace.height;
	if (sx < sy)
		smin = sx;
	else
		smin = sy;
	tx = viewSpace.x - smin * logicSpace.x + (viewSpace.width - smin * logicSpace.width) / 2;
	ty = viewSpace.y - smin * logicSpace.y + (viewSpace.height - smin * logicSpace.height) / 2;

	return glm::transpose(glm::mat3(
		smin, 0.0f, tx,
		0.0f, smin, ty,
		0.0f, 0.0f, 1.0f));
}

void GeometryWars::SetViewportArea(const ViewportSpace & viewSpace, glm::vec3 colorColor, bool clear)
{
	glViewport(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

	glEnable(GL_SCISSOR_TEST);
	glScissor(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

	// Clears the color buffer (using the previously set color) and depth buffer
	glClearColor(colorColor.r, colorColor.g, colorColor.b, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_SCISSOR_TEST);

	camera->SetOrthographic((float)viewSpace.x, (float)(viewSpace.x + viewSpace.width), (float)viewSpace.y, (float)(viewSpace.y + viewSpace.height), 0.1f, 400);
	camera->Update();
}

void GeometryWars::FrameStart()
{
	// Clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

void GeometryWars::actualizeEnemyShips(float deltaTime) {
	
	std::set<EnemyShip> newEnemies;

	for (EnemyShip s : enemies)
	{
		float radiansOx = atan2(s.getX() - ship.getX(), s.getY() - ship.getY());
		float orient = -radiansOx + (float)M_PI * 0.5f;
		glm::vec3 v(s.getX(), s.getY(), 1);
		v = Transform2D::Translate(-sin(radiansOx) * deltaTime * s.velocity, -cos(radiansOx) * deltaTime * s.velocity) * v;
		EnemyShip newShip(v, s.sideLength, s.velocity, orient, s.typeShipe);
		newShip.lives = s.lives;
		newEnemies.insert(newShip);
	}

	enemies = newEnemies;
}

void GeometryWars::addEnemyShip() {

	lastEnemyOccurrence = absoluteTime;
	glm::vec3 v(ship.getX(), ship.getY(), 1);
	float alpha = (float)rand();
	int type = rand() % 2 + 1;
	v = Transform2D::Translate(sin(alpha) * enemyRadius, cos(alpha) * enemyRadius) * v;
	EnemyShip s(v, length, (rand() % 100) / 300.0f +  velocityEnemySheep, 0, type);

	if (type == 1)
		s.lives = 1;

	//orientation will be modified immediately on InputUpdate, it does not matter now, so 0 is fine
	enemies.insert(s);
}

void GeometryWars::moveShipAfterCursor() {

	glm::ivec2 mouse = window->GetCursorPosition();
	glm::vec3 v(ship.getX(), -ship.getY(), 1);//coordointaes y are opposite
	v = visMatrix * v;

	float radiansOx = atan2(mouse[0] - v[0], mouse[1] - v[1]);
	ship.orientation = radiansOx + (float)M_PI * 0.5f;
}

void GeometryWars::testCollisions() {

	std::set<EnemyShip> newEnemies;

	for (EnemyShip s : enemies)
	{
		//test for collisions between ships
		if ((s.getX() - ship.getX()) * (s.getX() - ship.getX()) +
			(s.getY() - ship.getY()) * (s.getY() - ship.getY()) <= (s.sideLength / 2 + ship.sideLength / 2) * (s.sideLength / 2 + ship.sideLength / 2))
		{
			ship.lives--;
			continue;
		}
			
		bool collision = false;
		//collision with bullets
		for (Bullet b : ship.bullets)
			if ((s.getX() - b.centroid.x) * (s.getX() - b.centroid.x) +
				(s.getY() - b.centroid.y) * (s.getY() - b.centroid.y) <= (s.sideLength / 2 + bulletSize / 2) * (s.sideLength / 2 + bulletSize / 2)) {
	
				if (s.lives == 1)
				{
					ship.bullets.erase(b);
					collision = true;
					break;
				}
				else 
				{
					s.lives--;
					s.velocity *= speedUp;
					s.sideLength = s.sideLength * 0.5f;
					s.timeOfHitting = (float)Engine::GetElapsedTime();
					ship.bullets.erase(b);
					break;
				}
			}
		
		if (collision == false) 
			newEnemies.insert(s);
	}

	std::set<Life> newLives;
	for (Life l : lives)
	{
		if ((ship.getX() - l.centroid.x) * (ship.getX() - l.centroid.x) +
			(ship.getY() - l.centroid.y) * (ship.getY() - l.centroid.y) > (ship.sideLength / 2 + l.size / 2) * (ship.sideLength / 2 + l.size / 2))
			newLives.insert(l);
		else
			ship.lives++;

	}

	lives = newLives;
	enemies = newEnemies;

}

void GeometryWars::actualizeBullets(float deltaTime) {
	
	std::set<Bullet> newBullets;

	for (Bullet b : ship.bullets)
	{
		glm::vec3 centroid = Transform2D::Translate(-sin(b.orientation) * deltaTime * velocityBullet, 
					-cos(b.orientation) * deltaTime * velocityBullet) * b.centroid;
		
		Bullet newB(centroid, b.orientation);
		newBullets.insert(newB);
	}

	ship.bullets = newBullets;
}


void GeometryWars::actualizeLives() {
	
	set<Life> newLives;
	for (Life l : lives) {
		if (rand() % 3000 != 1213)
			newLives.insert(l);
	}

	lives = newLives;
}

void GeometryWars::Update(float deltaTimeSeconds)
{

	absoluteTime += deltaTimeSeconds;

	glm::ivec2 resolution = window->GetResolution();
	
	// Sets the screen area where to draw - the left half of the window
	viewSpace = ViewportSpace(0, 0, resolution.x, resolution.y);
	if (freeze == true)
		SetViewportArea(viewSpace, glm::vec3(std::min( (Engine::GetElapsedTime() - freezingTime) * 0.5, (double)1), 0, 0), true);
	else
		SetViewportArea(viewSpace, colorBackgroundInitial, true);
	
	// Compute the 2D visualization matrix
	visMatrix = glm::mat3(1);
	visMatrix *= VisualizationTransf2DUnif(logicSpace, viewSpace);

	if (absoluteTime - lastModification > intervalModifyTime)
	{
		lastModification = absoluteTime;
		occurrenceInterval *= multiplyEnemies;

		if (occurrenceInterval < 0.5f)
			occurrenceInterval = 0.5f;
	}


	if (freeze == false)
	{
		
		if (absoluteTime - lastEnemyOccurrence > occurrenceInterval) {
			lastEnemyOccurrence = absoluteTime;
			addEnemyShip();
		}

		if(rand() % 600 == 0)
			addLife();

		actualizeLives();
		actualizeEnemyShips(deltaTimeSeconds);
		actualizeBullets(deltaTimeSeconds);
		testCollisions();
		ship.computeNextShip(deltaTimeSeconds);
		moveShipAfterCursor();
	}

	DrawScene(visMatrix);
}

void GeometryWars::FrameEnd()
{

}

void GeometryWars::drawShip(Ship ship, std::string meshName, float dimScale = 1) {
	

	if (ship.lives <= 0 && freeze == false)
	{
		freeze = true;
		freezingTime = (float)Engine::GetElapsedTime();
	}

	float distFromTriangles = sqrt(3.0f) / 2.0f  * ship.sideLength / 3.0f;
	

	modelMatrix = visMatrix;
	modelMatrix *= Transform2D::Translate(ship.getX(), ship.getY());
	modelMatrix *= Transform2D::Scale(dimScale, dimScale);
	modelMatrix *= Transform2D::Rotate(ship.orientation);
	RenderMesh2D(meshes[meshName], shaders["VertexColor"], modelMatrix);

}

void GeometryWars::drawBullets() {

	for (Bullet b : ship.bullets) {
		modelMatrix = visMatrix;
		modelMatrix *= Transform2D::Translate(b.centroid.x, b.centroid.y);
		RenderMesh2D(meshes["bullet1"], shaders["VertexColor"], modelMatrix);
	}
}

void GeometryWars::drawScoreBoard(MainShip ship) {

	for (int i = 0; i < ship.lives; ++i) {
		modelMatrix = visMatrix;
		modelMatrix *= Transform2D::Translate(logicSpace.x + logicSpace.width * 1.1f + i * logicSpace.width * 0.02f,
											  logicSpace.y + logicSpace.height * 0.90f);
		RenderMesh2D(meshes["rectangle1"], shaders["VertexColor"], modelMatrix);
	}
}

void GeometryWars::addLife() 
{

	float x = logicSpace.x + rand() % 100 / 100.0f * logicSpace.width;
	float y = logicSpace.y + rand() % 100 / 100.0f * logicSpace.height;

	lives.insert(Life(glm::vec3(x, y, 1), length));
}

void GeometryWars::drawLife(std::string meshName1, std::string meshName2)
{

	for (Life l : lives)
	{
		modelMatrix = visMatrix;
		modelMatrix *= Transform2D::Translate(l.centroid.x, l.centroid.y);
		RenderMesh2D(meshes[meshName1], shaders["VertexColor"], modelMatrix);


		modelMatrix = visMatrix;
		modelMatrix *= Transform2D::Translate(l.centroid.x, l.centroid.y);
		RenderMesh2D(meshes[meshName2], shaders["VertexColor"], modelMatrix);
	}
}

void GeometryWars::DrawScene(glm::mat3 visMatrix)
{
	drawScoreBoard(ship);
	drawShip(ship, "mainShip" );

	for (EnemyShip s : enemies)
		if (s.typeShipe == 2)
		{
			if (s.lives == 2)
				drawShip(s, "enemyShip1");
			else
			{
				float dimScale = (1.0f - (float)std::min(Engine::GetElapsedTime() - s.timeOfHitting, 0.25));
				drawShip(s, "enemyShip2", dimScale);
			}
		}
		else
			drawShip(s, "enemyShip3");

	drawBullets();
	drawLife("life1", "life2");
}

void GeometryWars::OnInputUpdate(float deltaTime, int mods)
{
	//TODO move the logic window with W, A, S, D (up, left, down, right)
	//TODO zoom in and zoom out logic window with Z and X
}

void GeometryWars::OnKeyPress(int key, int mods)
{
	if (key == GLFW_KEY_W)
		ship.moveUp = true;
	if (key == GLFW_KEY_A)
		ship.moveLeft = true;
	if (key == GLFW_KEY_S)
		ship.moveDown = true;
	if (key == GLFW_KEY_D)
		ship.moveRight = true;
}

void GeometryWars::OnKeyRelease(int key, int mods)
{
	if (key == GLFW_KEY_W)
		ship.moveUp = false;
	if (key == GLFW_KEY_A)
		ship.moveLeft = false;
	if (key == GLFW_KEY_S)
		ship.moveDown = false;
	if (key == GLFW_KEY_D)
		ship.moveRight = false;
}

void GeometryWars::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{	
}

void GeometryWars::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{

	if (button == GLFW_MOUSE_BUTTON_3 && freeze == false) {

		glm::vec3 v(ship.getX(), -ship.getY(), 1);//coordointaes y are opposite
		v = visMatrix * v;

		float radiansOx = atan2(mouseX - v[0], mouseY - v[1]);
		float orientation = -radiansOx;

		ship.bullets.insert(Bullet(glm::vec3(ship.getX(), ship.getY(), 1.0f), orientation));
	}
} 

void GeometryWars::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void GeometryWars::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}