#pragma once
#include "..\nclgl\scenenode.h"
#include "..\nclgl\OBJMesh.h"

class CubeRobot : public SceneNode {
public:
	CubeRobot(void);
	~CubeRobot(void);
	virtual void Update(float msec);
	static void CreateCube() {
		OBJMesh* m = new OBJMesh();
		m->LoadOBJMesh(MESHDIR "cube.obj");
		cube = m;
	}

	static void CreateMan() {
		OBJMesh* m = new OBJMesh();
		m->LoadOBJMesh(MESHDIR"nanosuit.obj");
		Man = m;
	}
	static void DeleteCube() { delete cube; }
	static void DeleteMan() { delete Man; }
protected:
	static Mesh* Man;
	static Mesh* cube;
	SceneNode* head;
	SceneNode* leftArm;
	SceneNode* rightArm;
};
