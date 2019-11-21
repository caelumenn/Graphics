#pragma once
#include "../nclgl/OGLRenderer.h"
#include "../nclgl/Camera.h"
#include "../nclgl/OBJmesh.h"
#include "../nclgl/heightmap.h"
#include "../nclgl/Light.h"
#include "CubeRobot.h"
#include "../nclgl/MD5Mesh.h"
#include "../nclgl/MD5Node.h"
class Renderer : public OGLRenderer
{
public:
	Renderer(Window& parent);
	virtual ~Renderer(void);

	virtual void RenderScene();
	virtual void UpdateScene(float msec);

	void MoveLight(float f);
	void DrawNode(SceneNode* n);
	void Drawhell();

	OBJMesh* sphere; // Light volume
	HeightMap* heightMap;
	Camera* camera; // Our usual camera

	Shader* lightShader;
	Shader* reflectShader;
	Shader* skyboxShader;
	Shader* cubeShader;
	Shader* hellShader;
	Light** light;
	GLuint cubeMap;
	Mesh* water;
	float rainRotate;
	SceneNode* root;
	MD5FileData* hellData;
	MD5Node* hellNode;

protected:
	void DrawHeightMap();
	void DrawMesh();
	void DrawSkybox();
	void DrawRain();
};

