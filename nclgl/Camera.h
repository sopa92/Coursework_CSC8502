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

class Camera {
public:
	Camera(void) {
		yaw = 0.0f;
		pitch = 0.0f;
		SetStopPointsPosition();
		SetStopPointsPitchYaw();
	}

	Camera(float pitch, float yaw, Vector3 position) {
		this->pitch = pitch;
		this->yaw = yaw;
		this->position = position;
	}

	~Camera(void) {}

	void UpdateCamera(float msec = 10.0f);

	Matrix4 BuildViewMatrix();

	Vector3 GetPosition() const { return position; }
	void SetPosition(Vector3 val) { position = val; }

	float GetYaw()   const { return yaw; }
	void SetYaw(float y) { yaw = y; }

	float GetPitch() const { return pitch; }
	void SetPitch(float p) { pitch = p; }

	void SetStopPointsPosition();
	void SetStopPointsPitchYaw();


protected:
	float	yaw;
	float	pitch;
	Vector3 position;
	float speed = 0.0f;
	int currentPosition = 0;
	float camera_height = 150.0f;
	bool paused = false;
	vector<Vector3> cameraStopPoints;
	vector<Vector2> stopPointsPitchYaw;
	bool isCameraAutomated = true;

	float l_interp(float v0, float v1, float t) {
		return (1 - t) * v0 + t * v1;
	}
	void SetTransportOptions() {
		if (!isCameraAutomated) {
			paused = true;
			speed = 0.0f;
		}
		isCameraAutomated = true;
	}

};