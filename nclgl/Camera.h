/******************************************************************************
Class:Camera
Description:FPS-Style camera. Uses the mouse and keyboard from the Window
class to get movement values!
Automated camera, moving around the place, with specific stop points.
*//////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Window.h"
#include "Matrix4.h"
#include "Vector3.h"
//#include "..\glm\gtc\matrix_transform.hpp"

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

	Matrix4 BuildViewMatrix();

	Vector3 GetPosition() const { return position; }
	void SetPosition(Vector3 val) { position = val; }

	Matrix4 MoveCameraAround(float msec);
	void SetInitialPosition(Vector3 val) { position = val; cameraStopPoints[0] = val; }

	int GetCurrentPosition() { return currentPosition; }
	void SetCurrentPosition(int newPosition) { currentPosition = newPosition; }

	bool GetStatePaused() { return paused; }
	void SetStatePaused(bool newState) { paused = newState; }

	bool GetDirection() { return toNextPosition; }
	void SetDirectionToNext(bool goToNext) { toNextPosition = goToNext; }

	float	GetYaw()   const { return yaw; }
	void	SetYaw(float y) { yaw = y; }

	float	GetPitch() const { return pitch; }
	void	SetPitch(float p) { pitch = p; }

	Vector3 cameraTarget;
protected:
	float	yaw;
	float	pitch;
	Vector3 position;
	float speed = 0.2f;
	int currentPosition = 0;
	float y = 200.0f;
	bool toNextPosition = true;
	bool paused = false;
	float diffX = 0.0f;
	float diffZ = 0.0f;
	Vector3 cameraStopPoints[18] = {
		Vector3(0,y,0),
		Vector3(2448,y,3863), 
		Vector3(2704,y,3624),
		Vector3(2699,y,3121),
		Vector3(2686,y,2832),
		Vector3(2864,y,2717),
		Vector3(2649,y,2673),
		Vector3(2234,y,2575),
		Vector3(1631,y,2800),
		Vector3(1332,y,2698),
		Vector3(1297,y,2155),
		Vector3(1932,y,2132),
		Vector3(2329,y,2637),
		Vector3(2392,y,3144),
		Vector3(1980,y,3368),
		Vector3(1252,y,3524),
		Vector3(983,y,3660),
		Vector3(983,y,3660)
	};
	float yawStopPoints[18] = {
		0.0f,
		353.0f,
		353.0f,
		333.0f,
		308.0f,
		62.0f,
		89.0f,
		77.0f,
		9.0f,
		313.0f,
		232.0f,
		196.0f,
		193.0f,
		126.0f,
		99.0f,
		76.0f,
		295.0f,
		288.0f
	};
	float pitchStopPoints[18] = {
		0.0f,
		-13.65f,
		-11.76f,
		-12.95f,
		-16.94f,
		-12.46f,
		-12.88f,
		-13.3f,
		-13.72f,
		-14.7f,
		-13.58f,
		-10.5f,
		-15.12f,
		-16.94f,
		-20.44f,
		-20.23f,
		-14.21f,
		23.94f
	};
};