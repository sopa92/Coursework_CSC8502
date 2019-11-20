#include "Camera.h"

/*
Polls the camera for keyboard / mouse movement.
Should be done once per frame! Pass it the msec since
last frame (default value is for simplicities sake...)
*/
void Camera::UpdateCameraManually(float msec)	{
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
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_Y)) {
		cout << yaw << endl;
	}
	msec *= speed;

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

	if(Window::GetKeyboard()->KeyDown(KEYBOARD_SHIFT)) {
		position.y += msec;
	}
	if(Window::GetKeyboard()->KeyDown(KEYBOARD_SPACE)) {
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

void Camera::MoveCameraAround(float msec) {

	if (currentPosition + 1 < (sizeof(cameraStopPoints) / sizeof(cameraStopPoints[0]))) {
		Vector3 current = GetPosition();
		//cout << "current : " << current << endl;

		Vector3 next = cameraStopPoints[currentPosition + 1];
		//cout << "next : " << next << endl;
		
		float diffX = next.x - current.x;
		if (diffX != 0) {
			if (diffX > 0 && diffX <= 1.0f) {
				diffX = 2.0f;
			}
			else if (diffX < 0 && diffX >= -1.0f) {
				diffX = -2.0f;
			}
			position += Matrix4::Rotation(yaw, Vector3(0, 1, 0)) * Vector3(diffX, 0, 0) * 0.003f;
			if ((position.x > next.x && diffX > 0) || (position.x < next.x && diffX < 0))
				position.x = next.x;
		}

		if (next.y - current.y != 0) {
			position.y += next.y - current.y;
		}

		float diffZ = next.z - current.z;
		if (diffZ != 0) {
			if (diffZ > 0 && diffZ <= 1.0f) {
				diffZ = 2.0f;
			}
			else if (diffZ < 0 && diffZ >= -1.0f) {
				diffZ = -2.0f;
			}
			position += Matrix4::Rotation(yaw, Vector3(0, 1, 0)) * Vector3(0, 0, diffZ) * 0.003f;
			if ((position.z > next.z && diffZ > 0) || (position.z < next.z && diffZ < 0))
				position.z = next.z;
		}

		if (next == position) {
			currentPosition++;
		}
	}
}
