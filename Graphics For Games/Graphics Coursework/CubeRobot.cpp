#include "CubeRobot.h"

Mesh * CubeRobot::cube = NULL;
Mesh* CubeRobot::Man = NULL;

CubeRobot::CubeRobot(void) {
	// Optional , uncomment if you want a local origin marker !
	// SetMesh ( cube );
	transform = Matrix4::Translation(Vector3(2200, -100, 1000))*Matrix4::Rotation(-90.f,Vector3(0,1,0));

	SceneNode * body = new SceneNode(cube, Vector4(1, 0, 0, 1)); // Red !
	body -> SetModelScale(Vector3(100, 100, 100));
	body -> SetTransform(Matrix4::Translation(Vector3(0, 200, 0)));
	AddChild(body);
	head = new SceneNode(cube, Vector4(0, 1, 0, 1)); // Green !
	head -> SetModelScale(Vector3(150, 150, 150));
	head -> SetTransform(Matrix4::Translation(Vector3(0, 150, 0)));
	body -> AddChild(head);
	
	leftArm = new SceneNode(cube, Vector4(0, 0, 1, 1)); // Blue !
	leftArm -> SetModelScale(Vector3(100, 100, 100));
	leftArm -> SetTransform(Matrix4::Translation(Vector3(0, 100, 0)));
	head -> AddChild(leftArm);

	rightArm = new SceneNode(Man, Vector4(1, 1, 1, 1)); 
	rightArm -> SetModelScale(Vector3(50, 50, 50));
	rightArm -> SetTransform(Matrix4::Translation(Vector3(0, 100, 0)));
	leftArm -> AddChild(rightArm);
	
}//-msec / 10.0f,

void CubeRobot::Update(float msec) {
	head->SetTransform(head->GetTransform() * Matrix4::Rotation(- msec / 10.0f, Vector3(0, 1, 0)));
	//SetTransform(head->GetTransform() * Matrix4::Scale(Vector3(1.001f, 1.001f, 1.001f)));
	SceneNode::Update(msec);
}
