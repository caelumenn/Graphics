#include "Camera.h"
#include <cmath>

cameraLocation Camera::FixedCameraLocations[4] = {{-10, 270, Vector3(-2100, 1000, 2050)},
												  {-10, 0, Vector3(2050, 1000, 6200)},
												  {-10, 90, Vector3(6200, 1000, 2050)},
												  {-10, 180, Vector3(2050, 1000, -2050)},
};

Camera::Camera(void) {
	pitch = 0.0f;
	yaw = 0.0f;
	fixedMode = false;
	fixedMove = false;
	currentLocation = 0;
	t = 0;
}

Camera::Camera(float pitch, float yaw, Vector3 position) {
	this->pitch = pitch;
	this->yaw = yaw;
	this->position = position;
	fixedMode = false;
	fixedMove = false;
	currentLocation = 0;
	t = 0;
}

/*
Polls the camera for keyboard / mouse movement.
Should be done once per frame! Pass it the msec since
last frame (default value is for simplicities sake...)
*/
void Camera::UpdateCamera(float msec)	{
	//Update the mouse by how much
	pitch -= (Window::GetMouse()->GetRelativePosition().y);
	yaw	  -= (Window::GetMouse()->GetRelativePosition().x);

	//Bounds check the pitch, to be between straight up and straight down ;)
	pitch = min(pitch,90.0f);
	pitch = max(pitch,-90.0f);

	if(yaw <0) {
		yaw+= 360.0f;
	}
	if(yaw > 360.0f) {
		yaw -= 360.0f;
	}

	msec *= 5.0f;

	if(Window::GetKeyboard()->KeyDown(KEYBOARD_W)) {
		position += (Matrix4::Rotation(yaw, Vector3(0,1,0)) * Vector3(0,0,-1) * msec)/3;
	}
	if(Window::GetKeyboard()->KeyDown(KEYBOARD_S)) {
		position -= (Matrix4::Rotation(yaw, Vector3(0,1,0)) * Vector3(0,0,-1) * msec) / 3;
	}

	if(Window::GetKeyboard()->KeyDown(KEYBOARD_A)) {
		position += (Matrix4::Rotation(yaw, Vector3(0,1,0)) * Vector3(-1,0,0) * msec) / 3;
	}
	if(Window::GetKeyboard()->KeyDown(KEYBOARD_D)) {
		position -= (Matrix4::Rotation(yaw, Vector3(0,1,0)) * Vector3(-1,0,0) * msec) / 3;
	}

	if(Window::GetKeyboard()->KeyDown(KEYBOARD_SHIFT)) {
		position.y += msec/5;
	}
	if(Window::GetKeyboard()->KeyDown(KEYBOARD_SPACE)) {
		position.y -= msec/5;
	}
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_F)) {
		fixedMode = true;
		yaw = FixedCameraLocations[currentLocation].yaw;
		pitch = FixedCameraLocations[currentLocation].pitch;
		position = FixedCameraLocations[currentLocation].position;
	}
}

void Camera::UpdateFixedCamera(float msec) {

	if(!fixedMove){
		t += (0.25f / (1000 / msec))/2;
		if (t >= 1.0f) { // Segment completes
			t = 0;
			currentLocation = (currentLocation + 1) % numberCameraLocations;
		}
		calculateCatmoll_Rom();
	}

	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_F)) {
		fixedMode = false;
	}
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_M)) {
		switchFixedMove();
	}
}

// The author of the code is javidx9, from video https://www.youtube.com/watch?v=9_aJGUTePYo
// I have adjusted it to fit into the Camera class
void Camera::calculateCatmoll_Rom() {

	int p0, p1, p2, p3;
	p1 = currentLocation;
	p2 = (p1 + 1) % numberCameraLocations;
	p3 = (p2 + 1) % numberCameraLocations;
	p0 = p1 >= 1 ? p1 - 1 : numberCameraLocations - 1;

	float tt = t * t;
	float ttt = tt * t;
	float q1 = -ttt + 2.0f * tt - t;
	float q2 = 3.0f * ttt - 5.0f * tt + 2.0f;
	float q3 = -3.0f * ttt + 4.0f * tt + t;
	float q4 = ttt - tt;

	yaw = (FixedCameraLocations[p0].yaw * q1 + FixedCameraLocations[p1].yaw * q2 +
		   FixedCameraLocations[p2].yaw * q3 + FixedCameraLocations[p3].yaw * q4) * 0.5f;
	pitch = (FixedCameraLocations[p0].pitch * q1 + FixedCameraLocations[p1].pitch * q2 +
		   FixedCameraLocations[p2].pitch * q3 + FixedCameraLocations[p3].pitch * q4) * 0.5f;
	position = (FixedCameraLocations[p0].position * q1 + FixedCameraLocations[p1].position * q2 +
		   FixedCameraLocations[p2].position * q3 + FixedCameraLocations[p3].position * q4) * 0.5f;
}

/*
Generates a view matrix for the camera's viewpoint. This matrix can be sent
straight to the shader...it's already an 'inverse camera' matrix.
*/
Matrix4 Camera::BuildViewMatrix()	{
	//Why do a complicated matrix inversion, when we can just generate the matrix
	//using the negative values ;). The matrix multiplication order is important!
	return	Matrix4::Rotation(-pitch, Vector3(1,0,0)) * 
			Matrix4::Rotation(-yaw, Vector3(0,1,0)) * 
			Matrix4::Translation(-position);
};

void Camera::switchFixedMove() {
	if (fixedMove)
		fixedMove = false;
	else
		fixedMove = true;
}
