#pragma once
#include "../nclgl/OGLRenderer.h"
#include "../nclgl/Camera.h"
#include "../nclgl/OBJmesh.h"
#include "../nclgl/heightmap.h"
#include "../nclgl/Light.h"
class Renderer : public OGLRenderer
{
public:
	Renderer(Window& parent);
	virtual ~Renderer(void);

	virtual void RenderScene();
	virtual void UpdateScene(float msec);

	void MoveLight(float f);

	OBJMesh* sphere; // Light volume
	HeightMap* heightMap;
	Camera* camera; // Our usual camera

	Shader* lightShader;
	Shader* reflectShader;
	Shader* skyboxShader;
	Light* light;
	GLuint cubeMap;
	Mesh* rain;
	float rainRotate;
protected:
	void DrawHeightMap();
	void DrawMesh();
	void DrawSkybox();
	void DrawRain();
};

