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

class Camera {
public:
	Camera(void) {
		yaw = 0.0f;
		pitch = 0.0f;
	}

	Camera(float pitch, float yaw, Vector3 position) {
		this->pitch = pitch;
		this->yaw = yaw;
		this->position = position;
	}

	~Camera(void) {}

	void UpdateCameraManually(float msec = 10.0f);

	//Builds a view matrix for the current camera variables, suitable for sending straight
	//to a vertex shader (i.e it's already an 'inverse camera matrix').
	Matrix4 BuildViewMatrix();

	//Gets position in world space
	Vector3 GetPosition() const { return position; }
	//Sets position in world space
	void SetPosition(Vector3 val) { position = val; }

	void MoveCameraAround(float msec);
	void SetInitialPosition(Vector3 val) { position = val; cameraStopPoints[0] = val; }

	int GetCurrentPosition() { return currentPosition; }
	//Gets yaw, in degrees
	float	GetYaw()   const { return yaw; }
	//Sets yaw, in degrees
	void	SetYaw(float y) { yaw = y; }

	//Gets pitch, in degrees
	float	GetPitch() const { return pitch; }
	//Sets pitch, in degrees
	void	SetPitch(float p) { pitch = p; }
protected:
	float	yaw;
	float	pitch;
	Vector3 position;
	float speed = 0.2f;
	int currentPosition = 0;
	Vector3 cameraStopPoints[17] = {
		Vector3(0,0,0),
		Vector3(2505,100,3883),
		Vector3(2700,100,3650),
		Vector3(2790,100,3362),
		Vector3(2823,100,3129),
		Vector3(2815,100,2786),
		Vector3(2794,100,2492),
		Vector3(2117,100,2538),
		Vector3(1581,100,2425),
		Vector3(2007,100,2691),
		Vector3(2280,100,3103),
		Vector3(1914,100,3384),
		Vector3(1562,100,3423),
		Vector3(1069,100,3466),
		Vector3(1044,100,3750),
		Vector3(1753,100,3715),
		Vector3(2262,100,3793)
	};
};