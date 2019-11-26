#include "Camera.h"

/*
Polls the camera for keyboard / mouse movement.
Should be done once per frame! Pass it the msec since
last frame (default value is for simplicities sake...)
*/
void Camera::UpdateCamera(float msec)	{
	//Update the mouse by how much
	pitch -= (Window::GetMouse()->GetRelativePosition().y);
	yaw -= (Window::GetMouse()->GetRelativePosition().x);

	//Bounds check the pitch, to be between straight up and straight down ;)
	pitch = min(pitch,90.0f);
	pitch = max(pitch,-90.0f);

	if(yaw <0) {
		yaw+= 360.0f;
	}
	if(yaw > 360.0f) {
		yaw -= 360.0f;
	}
	
	msec *= 0.5f;

	speed += 0.01f;

	if (isCameraAutomated) {
		if (!paused) {
			position = Vector3(l_interp(this->GetPosition().x, cameraStopPoints[currentPosition].x, 0.01f),
				l_interp(this->GetPosition().y, cameraStopPoints[currentPosition].y, 0.01f),
				l_interp(this->GetPosition().z, cameraStopPoints[currentPosition].z, 0.01f));

			this->SetPitch(l_interp(this->GetPitch(), stopPointsPitchYaw[currentPosition].x, 0.01f));
			this->SetYaw(l_interp(this->GetYaw(), stopPointsPitchYaw[currentPosition].y, 0.01f));
		}
		else {
			position = Vector3(l_interp(this->GetPosition().x, cameraStopPoints[currentPosition].x, 0.09f),
				l_interp(this->GetPosition().y, cameraStopPoints[currentPosition].y, 0.09f),
				l_interp(this->GetPosition().z, cameraStopPoints[currentPosition].z, 0.09f));

			this->SetPitch(l_interp(this->GetPitch(), stopPointsPitchYaw[currentPosition].x, 0.09f));
			this->SetYaw(l_interp(this->GetYaw(), stopPointsPitchYaw[currentPosition].y, 0.09f));
		}
		if (speed >= 3.0f) {
			currentPosition++;
			speed = 0.0f;
			if (paused) {
				isCameraAutomated = false;
			}
		}
		if (currentPosition >= cameraStopPoints.size()) {
			currentPosition = 0;
		}		
	}
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_SPACE)) {
		splitScreen = !splitScreen;
	}
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_C)) {
		isCameraAutomated = !isCameraAutomated;
	}
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_1)) {
		currentPosition = 0;
		SetTransportOptions();
	}
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_2)) {
		currentPosition = 2;
		SetTransportOptions();
	}
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_3)) {
		currentPosition = 4;
		SetTransportOptions();
	}
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_4)) {
		currentPosition = 6;
		SetTransportOptions();
	}
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_5)) {
		currentPosition = 8;
		SetTransportOptions();
	}
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_6)) {
		currentPosition = 10;
		SetTransportOptions();
	}
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_7)) {
		currentPosition = 12;
		SetTransportOptions();
	}
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_8)) {
		currentPosition = 14;
		SetTransportOptions();
	}
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_9)) {
		currentPosition = 15;
		SetTransportOptions();
	}

	if (Window::GetKeyboard()->KeyDown(KEYBOARD_P)) {
		cout << "position : " << position << endl;
		cout << "yaw : " << yaw << endl;
		cout << "pitch : " << pitch << endl;
	}
	if(Window::GetKeyboard()->KeyDown(KEYBOARD_W)) {
		position += Matrix4::Rotation(yaw, Vector3(0,1,0)) * Vector3(0,0,-1) * msec;
	}
	if(Window::GetKeyboard()->KeyDown(KEYBOARD_S)) {
		position -= Matrix4::Rotation(yaw, Vector3(0,1,0)) * Vector3(0,0,-1) * msec;
	}

	if(Window::GetKeyboard()->KeyDown(KEYBOARD_A)) {
		position += Matrix4::Rotation(yaw, Vector3(0,1,0)) * Vector3(-1,0,0) * msec;
	}
	if(Window::GetKeyboard()->KeyDown(KEYBOARD_D)) {
		position -= Matrix4::Rotation(yaw, Vector3(0,1,0)) * Vector3(-1,0,0) * msec;
	}

	if(Window::GetKeyboard()->KeyDown(KEYBOARD_UP)) {
		position.y += msec;
	}
	if(Window::GetKeyboard()->KeyDown(KEYBOARD_DOWN)) {
		position.y -= msec;
	}
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
}

void Camera::SetStopPointsPosition() {
	cameraStopPoints.push_back(Vector3(2418.82f, camera_height, 4112.0f));
	cameraStopPoints.push_back(Vector3(2692.82f, camera_height, 3834.0f));
	cameraStopPoints.push_back(Vector3(2770.82f, camera_height, 3430.0f));
	cameraStopPoints.push_back(Vector3(2974.82f, camera_height, 3066.0f));
	cameraStopPoints.push_back(Vector3(2616.65f, camera_height, 2997.1f));
	cameraStopPoints.push_back(Vector3(2309.53f, camera_height, 2742.31f));
	cameraStopPoints.push_back(Vector3(1937.06f, camera_height, 2635.88f));
	cameraStopPoints.push_back(Vector3(1596.12f, camera_height, 2713.65f));
	cameraStopPoints.push_back(Vector3(1276.49f, camera_height, 2513.88f));
	cameraStopPoints.push_back(Vector3(1425.42f, camera_height, 2093.87f));
	cameraStopPoints.push_back(Vector3(2136.65f, camera_height, 2510.0f));
	cameraStopPoints.push_back(Vector3(2392.11f, camera_height, 2872.89f));
	cameraStopPoints.push_back(Vector3(2215.33f, camera_height, 3139.57f));
	cameraStopPoints.push_back(Vector3(1922.03f, camera_height, 3440.39f));
	cameraStopPoints.push_back(Vector3(1588.99f, camera_height, 3526.52f));
	cameraStopPoints.push_back(Vector3(1103.56f, camera_height, 3415.49f));
	cameraStopPoints.push_back(Vector3(1922.03f, camera_height, 3440.39f));
	cameraStopPoints.push_back(Vector3(2418.82f, camera_height, 4112.0f));
}

void Camera::SetStopPointsPitchYaw()
{
	stopPointsPitchYaw.push_back(Vector2(0.0f, 0.0f));
	stopPointsPitchYaw.push_back(Vector2(0.0f, 0.0f));
	stopPointsPitchYaw.push_back(Vector2(0.0f, 0.0f));
	stopPointsPitchYaw.push_back(Vector2(-4.13f, 36.19f));
	stopPointsPitchYaw.push_back(Vector2(-2.66f, 45.15f));
	stopPointsPitchYaw.push_back(Vector2(-7.28f, 59.64f));
	stopPointsPitchYaw.push_back(Vector2(-11.97f, 42.98f));
	stopPointsPitchYaw.push_back(Vector2(-14.7f, 300.78f));
	stopPointsPitchYaw.push_back(Vector2(-11.76f, 285.87f));
	stopPointsPitchYaw.push_back(Vector2(-3.64f, 242.82f));
	stopPointsPitchYaw.push_back(Vector2(-1.68f, 215.87f));
	stopPointsPitchYaw.push_back(Vector2(-4.27f, 186.889f));
	stopPointsPitchYaw.push_back(Vector2(-19.18f, 147.059f));
	stopPointsPitchYaw.push_back(Vector2(-12.6f, 123.259f));
	stopPointsPitchYaw.push_back(Vector2(-8.96f, 104.499f));
	stopPointsPitchYaw.push_back(Vector2(-11.19f, 274.671f));
	stopPointsPitchYaw.push_back(Vector2(-12.6f, 295.0f));
	stopPointsPitchYaw.push_back(Vector2(0.0f, 40.0f));
}
