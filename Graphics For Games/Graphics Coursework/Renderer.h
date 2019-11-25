#pragma once
#include "../nclgl/OGLRenderer.h"
#include "../nclgl/Camera.h"
#include "../nclgl/OBJmesh.h"
#include "../nclgl/heightmap.h"
#include "../nclgl/Light.h"
#include "CubeRobot.h"
#include "../nclgl/MD5Mesh.h"
#include "../nclgl/MD5Node.h"

#define SHADOWSIZE 2048

class Renderer : public OGLRenderer
{
public:
	Renderer(Window& parent);
	virtual ~Renderer(void);

	virtual void RenderScene();
	virtual void UpdateScene(float msec);

	void DrawNode(SceneNode* n);
	void Drawhell();
	void DrawShadowScene(); // New !
	void DrawCombinedScene(); // New !
	void DrawHeightMap();
	void DrawMesh();
	void DrawSkybox();
	void DrawRain();

	HeightMap* heightMap;
	Camera* camera; // Our usual camera

	Shader* lightShader;
	Shader* reflectShader;
	Shader* skyboxShader;
	Shader* cubeShader;
	Shader* sceneShader;
	Shader* shadowShader;

	GLuint shadowTex;
	GLuint shadowFBO;
	Light** light;
	GLuint cubeMap;
	Mesh* water;
	float rainRotate;
	SceneNode* root;
	MD5FileData* hellData;
	MD5Node* hellNode;
	bool mode;
};

