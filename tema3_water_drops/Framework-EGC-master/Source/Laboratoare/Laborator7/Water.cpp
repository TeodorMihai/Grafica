#include "Water.h"
#include "Drop.h"

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>

using namespace std;

Water::Water()
{
}

Water::~Water()
{
}

void Water::Init()
{

	camera = new Mycamera::Camera();
	camera->Set(glm::vec3(0, 2, 3.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
	projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);

	{
		s.nameObject = "sphere";
		s.shader = "SimpleShader";
		s.position = glm::vec3(0, 1, 0);
		s.scale = 0.2f;
		s.color = colorSphere;
 
		Mesh* mesh = new Mesh("sphere");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		light.nameObject = "light";
		light.shader = "SimpleShader";
		light.position = glm::vec3(0, 1, 1);
		light.scale = 0.1f;
		light.color = colorLight;

		Mesh* mesh = new Mesh("light");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		plane.nameObject = "plane";
		plane.scale = 1.0f;
		plane.position = glm::vec3(0);
		plane.shader = "ShaderWater";
		plane.color = colorPlane;

		Mesh* meshPlane = CreatePlane("plane", glm::vec3(0), 100, 4, colorPlane);
		meshes[meshPlane->GetMeshID()] = meshPlane;
	}

	{
		Mesh* mesh = new Mesh("drop");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}
	// Create a shader program for drawing face polygon with the color of the normal
	{
		Shader *shader = new Shader("SimpleShader");
		shader->AddShader("Source/Laboratoare/Laborator7/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Laborator7/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	{
		Shader* shader = new Shader("ShaderWater");
		shader->AddShader("Source/Laboratoare/Laborator7/Shaders/WaterVertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Laborator7/Shaders/WaterFragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}
}

Mesh* Water::CreatePlane(const char* name, glm::vec3 centroid, int units, int dimension, glm::vec3 color) {
	
	//create vertices and indices, the logic of the mesh
	std::vector<VertexFormat> vertices;

	glm::vec3 leftUp = centroid + glm::vec3(-dimension * 0.5f, 0, -dimension * 0.5f);
	float dimUnit = (float) dimension / (float)units;
	distPoints = dimUnit;
	

	int** m = new int*[units];
	for (int i = 0; i < units; ++i)
		m[i] = new int[units];

	glm::vec3 nor = glm::vec3(0, 1, 0);

	for (int i = 0; i < units; ++i)
		for (int j = 0; j < units; ++j) {
			glm::vec3 pos = leftUp + glm::vec3((float)i * dimUnit, 0, (float)j * dimUnit);
			vertices.push_back(VertexFormat(pos, color, nor));
			m[i][j] = (int)vertices.size() - 1;
		}
	
	std::vector<unsigned short> indices;

	for (int i = 1; i < units; ++i)
		for (int j = 1; j < units; ++j) {
			indices.push_back(m[i - 1][j - 1]);
			indices.push_back(m[i - 1][j]);
			indices.push_back(m[i][j - 1]);

			indices.push_back(m[i - 1][j]);
			indices.push_back(m[i][j - 1]);
			indices.push_back(m[i][j]);
		}
	
	for (int i = 0; i < units; ++i)
		delete[] m[i];
	delete[] m;

	return CreateMesh(name, vertices, indices);
}

Mesh* Water::CreateMesh(const char *name, const std::vector<VertexFormat> &vertices, const std::vector<unsigned short> &indices) {
	
	//create the buffers for the GPU
	unsigned int VAO = 0;
	// Create the VAO and bind it 
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//  Create the VBO and bind it, vbo contains data about vertices (normal color etc)
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// Send vertices data into the VBO buffer (vertex buffer object)
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	// Create the IBO and bind it
	unsigned int IBO;
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	// TODO: Send indices data into the IBO buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

	// ========================================================================
	// This section describes how the GPU Shader Vertex Shader program receives data

	// set vertex position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

	// set vertex normal attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(1 * sizeof(glm::vec3)));

	// set texture coordinate attribute
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

	// set vertex color attribute
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));
	// ========================================================================

	// Unbind the VAO
	glBindVertexArray(0);

	// Check for OpenGL errors
	CheckOpenGLError();

	// Mesh information is saved into a Mesh object
	meshes[name] = new Mesh(name);
	meshes[name]->SetDrawMode(GL_LINE);
	meshes[name]->InitFromBuffer(VAO, static_cast<unsigned short>(indices.size()));
	meshes[name]->vertices = vertices;
	meshes[name]->indices = indices;
	return meshes[name];
	//tiny xml2
}

void Water::RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix, const glm::vec3 &color) {

	if (!mesh || !shader || !shader->GetProgramID())
		return;

	// render an object using the specified shader and the specified position
	glUseProgram(shader->program);

	// Set shader uniforms for light & material properties
	// TODO: Set light position uniform

	// TODO: Set eye position (camera position) uniform
	glm::vec3 eyePosition = camera->position;

	// TODO: Set material property uniforms (shininess, kd, ks, object color) 

	int loc_light_pos = shader->GetUniformLocation("light_position");
	glUniform3f(loc_light_pos, light.position.x, light.position.y, light.position.z);

	int loc_light_dir = shader->GetUniformLocation("light_direction");
	glUniform3f(loc_light_dir, lightDirection.x, lightDirection.y, lightDirection.z);

	int loc_eye_position = shader->GetUniformLocation("eye_position");
	glUniform3f(loc_eye_position, eyePosition.x, eyePosition.y, eyePosition.z);

	int loc_material_kd = shader->GetUniformLocation("material_kd");
	glUniform1f(loc_material_kd, materialKd);

	int loc_material_ks = shader->GetUniformLocation("material_ks");
	glUniform1f(loc_material_ks, materialKs);

	int loc_material_shininess = shader->GetUniformLocation("material_shininess");
	glUniform1ui(loc_material_shininess, materialShininess);

	int loc_object_color = shader->GetUniformLocation("object_color");
	glUniform3f(loc_object_color, color.x, color.y, color.z);


	if (strcmp(shader->GetName(), "ShaderWater") == 0) {

		int loc_dist = shader->GetUniformLocation("dim");
		glUniform1f(loc_dist, distPoints);

		int loc_len = shader->GetUniformLocation("len");
		glUniform1i(loc_len, (int)waves.size());

		int loc_current_time = shader->GetUniformLocation("currentTime");
		float currentTime = (float)Engine::GetElapsedTime();
		glUniform1f(loc_current_time, currentTime);


		for (unsigned i = 0; i < waves.size(); ++i) {

			string loc = "waves[" + to_string(i) + "]." + "amplitude";
			int loc_amplitude2 = shader->GetUniformLocation(&loc[0]);
			glUniform1f(loc_amplitude2, waves[i].amplitude);

			loc = "waves[" + to_string(i) + "]." + "waveLength";
			int loc_waveLength2 = shader->GetUniformLocation(&loc[0]);
			glUniform1f(loc_waveLength2, waves[i].waveLength);

			loc = "waves[" + to_string(i) + "]." + "speed";
			int loc_speed2 = shader->GetUniformLocation(&loc[0]);
			glUniform1f(loc_speed2, waves[i].speed);

			loc = "waves[" + to_string(i) + "]." + "pos";
			int loc_pos2 = shader->GetUniformLocation(&loc[0]);
			glUniform3f(loc_pos2, waves[i].pos.x, waves[i].pos.y, waves[i].pos.z);

			loc = "waves[" + to_string(i) + "]." + "startTime";
			int loc_startTime = shader->GetUniformLocation(&loc[0]);
			glUniform1f(loc_startTime, waves[i].startTime);


			loc = "waves[" + to_string(i) + "]." + "baseTimeM";
			int loc_baseTimeM = shader->GetUniformLocation(&loc[0]);
			glUniform1f(loc_baseTimeM, waves[i].baseTimeM);

			loc = "waves[" + to_string(i) + "]." + "baseTimeS";
			int loc_baseTimeS = shader->GetUniformLocation(&loc[0]);
			glUniform1f(loc_baseTimeS, waves[i].baseTimeS);
		}
	}


	// Bind model matrix
	GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	glm::mat4 viewMatrix = camera->GetViewMatrix();
	GLint loc_view_matrix = glGetUniformLocation(shader->program, "View");
	glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	// Bind projection matrix
	int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

void Water::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);	
}

void Water::draw(GameObject o, glm::mat4 m, int type) {
	
	if (type == 0)
		RenderSimpleMesh(meshes[o.nameObject], shaders[o.shader], m, o.color);
	else
		RenderMesh(meshes[o.nameObject], shaders[o.shader], m);
}

void Water::drawObjects() {

	draw(s, s.computeMatrix(), 0);
	draw(light, light.computeMatrix(), 0);
	draw(plane, plane.computeMatrix(), 0);


	for (Drop d : drops)
		draw(d, d.computeMatrix(), 0);
}

void Water::releaseDrop() {

	Drop d("drop", -1, scaleDrop * 5);
	d.position = s.position;
	d.scale = scaleDrop;
	d.shader = "SimpleShader";
	d.color = colorDrops;

	drops.push_back(d);
}

void Water::actualizeWaves(float deltaTime) {

	std::vector<Wave> newWaves;

	for (Wave w : waves)
		if ((float)Engine::GetElapsedTime() - w.startTime < waveLife * (1.0f / timeSpeed))
			newWaves.push_back(w);

	waves = newWaves;
	
	for (Wave& w : waves)
		w.amplitude = std::max(0.0f, w.amplitude - amplitudeAttenuation * deltaTime);
}

void Water::actualizeDrops(float deltaTime) {

	for (Drop& d : drops) {
			
		if (d.position.y < 0.0f) {
			d.direction *= -1;
			d.mass = d.mass / 2.0f;
			//v = 4 / 3 * pi * r ^ 3
			// x^3 = r^3/2 => x = r / pow(r, 1/3) 
			d.scale = d.scale / 1.25992104f;
			Wave w(amplitudeFromMass * d.mass + amplitudeFromVelocity * fabs(d.velocity),
				   waveLengthFromMass * d.mass + waveLengthFromVelocity * fabs(d.velocity),
				   speedFromVelocity * fabs(d.velocity) , d.position, (float)Engine::GetElapsedTime());
			w.speed = timeSpeed * w.actualSpeed;
			//some adjusment to keep the behaviour of the velocity normal
			d.velocity += (d.direction * -1 * fabs(d.position.y)) / fabs(d.velocity) * gravityAcceleration;

			d.position = glm::vec3(d.position.x, 0.0f, d.position.z);
			waves.push_back(w);
			d.jumps++;
		}
	}

	for (Drop& d : drops) {
		d.position = d.position + glm::vec3(0, d.direction * d.velocity * deltaTime + 
						-1 *  gravityAcceleration * deltaTime * deltaTime * 0.5, 0);
		d.velocity += -d.direction * gravityAcceleration * deltaTime;

		if (d.velocity > 0.0f)
			d.velocity -= frictionCoefficient * gravityAcceleration * deltaTime;
		else
			d.velocity += frictionCoefficient * gravityAcceleration * deltaTime;
	}


	std::vector<Drop> newDrops;

	for (Drop d : drops)
		if (d.jumps < maxJumps)
			newDrops.push_back(d);

	drops = newDrops;
}

void Water::Update(float deltaTimeSeconds) {

	actualizeDrops(deltaTimeSeconds * timeSpeed);
	actualizeWaves(deltaTimeSeconds * timeSpeed);
	drawObjects();
}

void Water::FrameEnd() {
	//DrawCoordinatSystem(camera->GetViewMatrix(), projectionMatrix);
}

// Documentation for the input functions can be found in: "/Source/Core/Window/InputController.h" or
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/Window/InputController.h

void Water::changeWavesSpeed() {

	for (Wave& w : waves) {
		w.baseTimeM = w.baseTimeM + ((float)Engine::GetElapsedTime() - w.startTime) * w.speed * 2 * (float)M_PI ;
		w.baseTimeS = w.baseTimeS + ((float)Engine::GetElapsedTime() - w.startTime) * w.speed * 2 * (float)M_PI / w.waveLength;
		w.startTime = (float)Engine::GetElapsedTime();
		w.speed = timeSpeed * w.actualSpeed;
	}

}

void Water::OnInputUpdate(float deltaTime, int mods)
{
	float speed = 2;

	if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
		
		glm::vec3 up = camera->up;
		glm::vec3 right = camera->right;
		glm::vec3 forward = -camera->forward;

		forward.y = 0;
		forward = normalize(forward);

		up.x = 0; up.z = 0;
		up = normalize(up);

		
		if (window->KeyHold(GLFW_KEY_W) && !window->KeyHold(GLFW_KEY_N)) light.position -= forward * deltaTime * speed;
		if (window->KeyHold(GLFW_KEY_A) && !window->KeyHold(GLFW_KEY_N)) light.position -= right * deltaTime * speed;
		if (window->KeyHold(GLFW_KEY_S) && !window->KeyHold(GLFW_KEY_N)) light.position += forward * deltaTime * speed;
		if (window->KeyHold(GLFW_KEY_D) && !window->KeyHold(GLFW_KEY_N)) light.position += right * deltaTime * speed;
		if (window->KeyHold(GLFW_KEY_E) && !window->KeyHold(GLFW_KEY_N)) light.position += up * deltaTime * speed;
		if (window->KeyHold(GLFW_KEY_Q) && !window->KeyHold(GLFW_KEY_N)) light.position -= up * deltaTime * speed;

		if (window->KeyHold(GLFW_KEY_UP) ) s.position -= forward * deltaTime * speed;
		if (window->KeyHold(GLFW_KEY_LEFT)) s.position -= right * deltaTime * speed;
		if (window->KeyHold(GLFW_KEY_DOWN)) s.position += forward * deltaTime * speed;
		if (window->KeyHold(GLFW_KEY_RIGHT)) s.position += right * deltaTime * speed;
		if (window->KeyHold(GLFW_KEY_M)) s.position += up * deltaTime * speed;
		if (window->KeyHold(GLFW_KEY_N)) s.position -= up * deltaTime * speed;
	}


	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {

		float cameraSpeed = 2.0f;

		if (window->KeyHold(GLFW_KEY_W)) {

			camera->TranslateForward(cameraSpeed * deltaTime);
		}

		if (window->KeyHold(GLFW_KEY_A)) {
			camera->TranslateRight(-cameraSpeed * deltaTime);
		}

		if (window->KeyHold(GLFW_KEY_S)) {
			camera->TranslateForward(-cameraSpeed * deltaTime);
		}

		if (window->KeyHold(GLFW_KEY_D)) {
			camera->TranslateRight(cameraSpeed * deltaTime);
		}

		if (window->KeyHold(GLFW_KEY_Q)) {
			camera->TranslateUpword(cameraSpeed * deltaTime);
		}

		if (window->KeyHold(GLFW_KEY_E)) {
			camera->TranslateUpword(-cameraSpeed * deltaTime);
		}
	}
}

void Water::OnKeyPress(int key, int mods) {

	if (key == GLFW_KEY_SPACE) {
		releaseDrop();
	}

	if (key == GLFW_KEY_O) {
		timeSpeed = timeSpeed + sensibiltyTime;
		changeWavesSpeed();
	}

	if (key == GLFW_KEY_P) {
		timeSpeed = std::max(0.0f, timeSpeed - sensibiltyTime);
		changeWavesSpeed();
	}
}

void Water::OnKeyRelease(int key, int mods) {
	// add key release event
}

void Water::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) {
	
	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {

		float sensivityOX = 0.001f;
		float sensivityOY = 0.001f;

		if (window->GetSpecialKeyState() == 0) {
			camera->RotateFirstPerson_OX(-deltaY * sensivityOY);
			camera->RotateFirstPerson_OY(-deltaX * sensivityOX);
		}
	}
}

void Water::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event

	if (button == GLFW_MOUSE_BUTTON_RIGHT) {
		releaseDrop();
	}
}

void Water::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Water::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) {

	timeSpeed = std::max(0.0f, timeSpeed + sensibiltyTime * offsetY);
	changeWavesSpeed();
}

void Water::OnWindowResize(int width, int height)
{
}
