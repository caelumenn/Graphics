#include "Renderer.h"

Renderer::Renderer(Window& parent) : OGLRenderer(parent) {
	
	sphere = new OBJMesh();
	if (!sphere->LoadOBJMesh(MESHDIR"DragonEye.obj")) {
		return;
	}

	camera = new Camera(0.0f, 0.0f, Vector3(10,10,1));

	currentShader = new Shader(SHADERDIR"TexturedVertex.glsl", SHADERDIR"TexturedFragment.glsl");

	projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);
	
	modelMatrix = Matrix4::Translation(Vector3(1,1,1)) *
		Matrix4::Rotation(0.0f, Vector3(0, 1, 0)) *
		Matrix4::Scale(Vector3(1, 1, 1));

	if (!currentShader->LinkProgram()) {
		return;
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);


	init = true;
}

Renderer::~Renderer(void) {
	delete sphere;
	delete camera;
}

void Renderer::RenderScene() {
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(currentShader->GetProgram());
	UpdateShaderMatrices();
	sphere->Draw();
	glUseProgram(0);

	SwapBuffers();
}

void Renderer::UpdateScene(float msec) {
	camera->UpdateCamera(msec);
	viewMatrix = camera->BuildViewMatrix();
	//rotation = msec * 0.1f;
}