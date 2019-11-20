/******************************************************************************
Class:Camera
Implements:
Author:Rich Davison	<richard.davison4@newcastle.ac.uk>
Description:FPS-Style camera. Uses the mouse and keyboard from the Window
class to get movement values!

-_-_-_-_-_-_-_,------,   
_-_-_-_-_-_-_-|   /\_/\   NYANYANYAN
-_-_-_-_-_-_-~|__( ^ .^) /
_-_-_-_-_-_-_-""  ""   

*//////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Window.h"
#include "Matrix4.h"
#include "Vector3.h"

#define numberCameraLocations 4

struct cameraLocation {
	float pitch;
	float yaw;
	Vector3 position;
};

class Camera	{
public:
	//Fixed Camera Location
	static cameraLocation FixedCameraLocations[numberCameraLocations];

	Camera(void);
	Camera(float pitch, float yaw, Vector3 position);

	~Camera(void){};

	void UpdateCamera(float msec = 10.0f);
	void UpdateFixedCamera(float msec = 10.0f);

	//Builds a view matrix for the current camera variables, suitable for sending straight
	//to a vertex shader (i.e it's already an 'inverse camera matrix').
	Matrix4 BuildViewMatrix();

	//Gets position in world space
	Vector3 GetPosition() const { return position;}
	//Sets position in world space
	void	SetPosition(Vector3 val) { position = val;}

	//Gets yaw, in degrees
	float	GetYaw()   const { return yaw;}
	//Sets yaw, in degrees
	void	SetYaw(float y) {yaw = y;}

	//Gets pitch, in degrees
	float	GetPitch() const { return pitch;}
	//Sets pitch, in degrees
	void	SetPitch(float p) {pitch = p;}

	void calculateCatmoll_Rom();

	bool fixedMode;
	bool fixedMove;
	void switchFixedMove();

protected:
	float	yaw;
	float	pitch;
	Vector3 position;

	unsigned short int currentLocation;
	float t; //interpolation value
};