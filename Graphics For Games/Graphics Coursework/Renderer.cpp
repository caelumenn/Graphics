#include "Renderer.h"
Renderer::Renderer(Window& parent) : OGLRenderer(parent) {
	camera = new Camera();
	CubeRobot::CreateCube(); // Important !
	CubeRobot::CreateMan();
	root = new SceneNode();
	root->AddChild(new CubeRobot());

	hellData = new MD5FileData(MESHDIR"hellknight.md5mesh");
	hellNode = new MD5Node(*hellData);
	hellData->AddAnim(MESHDIR"attack2.md5anim");
	hellNode->PlayAnim(MESHDIR"attack2.md5anim");

	heightMap = new HeightMap(TEXTUREDIR"terrain.raw");

	water = Mesh::GenerateQuad();

	camera->SetPosition(Vector3(RAW_WIDTH * HEIGHTMAP_X / 2.0f, 500.0f, RAW_WIDTH * HEIGHTMAP_X));

	light = new Light*[5];
	for (int i = 0; i < 5; i++) {
		light[i] = new Light;
	}
	light[0]->SetColour(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
	light[0]->SetRadius((RAW_WIDTH * HEIGHTMAP_X)/3.0f);
	light[1]->SetPosition(Vector3((RAW_HEIGHT * HEIGHTMAP_X / 2.0f), 500.0f, (RAW_HEIGHT * HEIGHTMAP_Z / 2.0f)));
	light[1]->SetColour(Vector4(0.0f, 0.0f, 1.0f, 1.0f));
	light[1]->SetRadius((RAW_WIDTH * HEIGHTMAP_X) / 3.0f);
	light[2]->SetPosition(Vector3((RAW_HEIGHT * HEIGHTMAP_X / 4.0f), 500.0f, (RAW_HEIGHT * HEIGHTMAP_Z - RAW_HEIGHT * HEIGHTMAP_Z / 4.0f)));
	light[2]->SetColour(Vector4(1.0f, 0.0f, 1.0f, 1.0f));
	light[2]->SetRadius((RAW_WIDTH * HEIGHTMAP_X) / 3.0f);
	light[3]->SetPosition(Vector3((RAW_HEIGHT * HEIGHTMAP_X / 4.0f), 500.0f, (RAW_HEIGHT * HEIGHTMAP_Z / 4.0f)));
	light[3]->SetColour(Vector4(1.0f, 1.0f, 0.0f, 1.0f));
	light[3]->SetRadius((RAW_WIDTH * HEIGHTMAP_X) / 3.0f);
	light[4]->SetPosition(Vector3((RAW_HEIGHT * HEIGHTMAP_X - RAW_HEIGHT * HEIGHTMAP_X / 16.0f), 500.0f, (RAW_HEIGHT * HEIGHTMAP_Z / 16.0f)));
	light[4]->SetColour(Vector4(1.0f, 0.0f, 0.0f, 1.0f));
	light[4]->SetRadius((RAW_WIDTH * HEIGHTMAP_X) / 3.0f);

	reflectShader = new Shader(SHADERDIR"PerPixelVertex.glsl", SHADERDIR"reflectFragment.glsl");
	skyboxShader = new Shader(SHADERDIR"skyboxVertex.glsl", SHADERDIR"skyboxFragment.glsl");
	lightShader = new Shader(SHADERDIR"PerPixelVertex.glsl", SHADERDIR"PerPixelFragment.glsl");
	cubeShader = new Shader(SHADERDIR"SceneVertex.glsl", SHADERDIR"SceneFragment.glsl");
	hellShader = new Shader(SHADERDIR"TexturedVertex.glsl", SHADERDIR"TexturedFragment.glsl");
	if (!reflectShader->LinkProgram() || !lightShader->LinkProgram() || !skyboxShader->LinkProgram() || !cubeShader->LinkProgram() || !hellShader->LinkProgram()) {
		return;
	}

	water->SetTexture(
		SOIL_load_OGL_texture(
			TEXTUREDIR "water.tga",
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS)
	);

	heightMap->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"Barren Reds.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	cubeMap = SOIL_load_OGL_cubemap(
		TEXTUREDIR"desertsky_lf.tga", TEXTUREDIR"desertsky_rt.tga",
		TEXTUREDIR "sky_up.jpg", TEXTUREDIR"desertsky_dn.tga",
		TEXTUREDIR "desertsky_ft.tga", TEXTUREDIR"desertsky_bk.tga",
		SOIL_LOAD_RGB,
		SOIL_CREATE_NEW_ID,
		0);

	if (!cubeMap || !water->GetTexture() || !heightMap->GetTexture()) {
		return;
	}

	SetTextureRepeating(water->GetTexture(), true);
	SetTextureRepeating(heightMap->GetTexture(), true);
	rainRotate = 0.0f;

	projMatrix = Matrix4::Perspective(1.0f, 15000.0f, (float)width / (float)height, 45.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	
	init = true;
}

Renderer ::~Renderer(void) {
	delete camera;
	delete heightMap;
	delete water;
	delete reflectShader;
	delete skyboxShader;
	delete lightShader;
	delete hellShader;
	delete[] light;
	delete root;
	delete hellData;
	delete hellNode;
	currentShader = 0;
}

void Renderer::UpdateScene(float msec) {
	camera->UpdateCamera(msec);
	camera->UpdateFixedCamera(msec);
	viewMatrix = camera->BuildViewMatrix();
	light[0]->SetPosition(camera->GetPosition());
	rainRotate += msec / 1000.0f;
	root->Update(msec);
	hellNode->Update(msec);
}

void Renderer::RenderScene() {
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	DrawSkybox();
	DrawHeightMap();
	DrawRain();
	DrawNode(root);
	Drawhell();
	SwapBuffers();
}

void Renderer::DrawSkybox() {
	glDepthMask(GL_FALSE);
	SetCurrentShader(skyboxShader);
	UpdateShaderMatrices();
	water->Draw();
	glUseProgram(0);
	glDepthMask(GL_TRUE);
}

void Renderer::DrawHeightMap() {
	SetCurrentShader(lightShader);
	SetShaderLight(light);
	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1, (float*)& camera->GetPosition());
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);

	modelMatrix = Matrix4::Translation(Vector3(0,0,0)) *
		Matrix4::Scale(Vector3(1, 1, 1)) *
		Matrix4::Rotation(0, Vector3(0.0f, 0.0f, 0.0f));
	textureMatrix.ToIdentity();

	UpdateShaderMatrices();
	heightMap->Draw();

	glUseProgram(0);
}

void Renderer::DrawRain() {
	SetCurrentShader(reflectShader);
	SetShaderLight(light);
	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1, (float*)& camera->GetPosition());
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "cubeTex"), 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);

	float heightX = (RAW_WIDTH * HEIGHTMAP_X / 2.0f);
	float heightY = 256 * HEIGHTMAP_Y / 4.0f;
	float heightZ = (RAW_HEIGHT * HEIGHTMAP_Z / 2.0f);

	modelMatrix = Matrix4::Translation(Vector3(heightX, heightY, heightZ)) *
		Matrix4::Scale(Vector3(heightX, 1, heightZ)) *
		Matrix4::Rotation(90, Vector3(1.0f, 0.0f, 0.0f));

	textureMatrix = Matrix4::Scale(Vector3(10.0f, 10.0f, 10.0f)) *
		Matrix4::Rotation(rainRotate, Vector3(0.0f, 0.0f, 1.0f));

	UpdateShaderMatrices();

	water->Draw();

	glUseProgram(0);
}

void Renderer::DrawNode(SceneNode* n) {
	SetCurrentShader(cubeShader);
	SetShaderLight(*light[0]);
	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1, (float*)& camera->GetPosition());
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);
	UpdateShaderMatrices();
	
	if (n->GetMesh()) {
		Matrix4 transform = n->GetWorldTransform() * Matrix4::Scale(n->GetModelScale());
		glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "modelMatrix"), 1, false, (float*)&transform);
		glUniform4fv(glGetUniformLocation(currentShader->GetProgram(), "nodeColour"), 1, (float*)&n->GetColour());
		glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "useTexture"), (int)n->GetMesh()->GetTexture());

		n->Draw(*this);
	}

	for (vector<SceneNode*>::const_iterator i = n->GetChildIteratorStart(); i != n->GetChildIteratorEnd(); ++i) {
		DrawNode(*i);
	}

	glUseProgram(0);
}

void Renderer::Drawhell() {
	SetCurrentShader(hellShader);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);
	modelMatrix = Matrix4::Translation(Vector3(1000, 50, 1000)) * Matrix4::Scale(Vector3(-10,10,10));
	textureMatrix.ToIdentity();
	UpdateShaderMatrices();

	hellNode->Draw(*this);

	glUseProgram(0);
}