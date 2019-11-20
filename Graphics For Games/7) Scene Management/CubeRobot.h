#pragma once
#include "..\nclgl\scenenode.h"
#include "..\nclgl\OBJMesh.h"

class CubeRobot : public SceneNode {
public:
	CubeRobot(void);
	~CubeRobot(void) {};
	virtual void Update(float msec);
	static void CreateCube() {
		Mesh * m = Mesh::GenerateCube();
		m->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"brick.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));
		cube = m;
	}
	static void DeleteCube() {delete cube;}
protected:
	static Mesh * cube;
	SceneNode * head;
	SceneNode * leftArm;
	SceneNode * rightArm;
};
