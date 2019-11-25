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
	
	msec *= speed;

	if (Window::GetKeyboard()->KeyDown(KEYBOARD_P)) {
		cout << "position : " << position << endl;
		cout << "yaw : " << yaw << endl;
		cout << "msec : " << msec << endl;
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
	if (paused)
		return;

	if (currentPosition + 1 < (sizeof(cameraStopPoints) / sizeof(cameraStopPoints[0]))) {
		Vector3 current = GetPosition();

		Vector3 nextPos = cameraStopPoints[currentPosition + 1];
		float nextYaw = yawStopPoints[currentPosition + 1];
		pitch = pitchStopPoints[currentPosition + 1];
		if (!toNextPosition) {
			nextPos = cameraStopPoints[currentPosition - 1];
			nextYaw = yawStopPoints[currentPosition - 1];
			pitch = pitchStopPoints[currentPosition - 1];
		}
		if (nextYaw > 180.0f && yaw <360.0f) {
			nextYaw -= 360.0f;
		}
		float diffYaw = nextYaw - yaw;
		if (diffYaw > 0) {
			yaw += 1.0f;
			if (yaw > nextYaw) {
				yaw = nextYaw;
			}
		}else if (diffYaw < 0) {
			yaw -= 1.0f;
			if (yaw < nextYaw) {
				yaw = nextYaw;
			}
		}
		msec *= speed/3;

		if (diffX == 0 && diffZ == 0) {
			diffX = (nextPos.x - current.x) / 100;
			diffZ = (nextPos.z - current.z) / 100;
		}
		if (diffX != 0 && diffZ == 0) {
			if (diffX > 0) {
				position -= Matrix4::Rotation(yaw, Vector3(0, 1, 0)) * Vector3(-1, 0, 0) * msec;
			}
			else {
				position += Matrix4::Rotation(yaw, Vector3(0, 1, 0)) * Vector3(-1, 0, 0) * msec;
			}
			position.z = current.z;
		}
		else if (diffZ != 0 && diffX == 0) {
			if (diffZ > 0) {
				position -= Matrix4::Rotation(yaw, Vector3(0, 1, 0)) * Vector3(0, 0, -1) * msec;
			}
			else {
				position += Matrix4::Rotation(yaw, Vector3(0, 1, 0)) * Vector3(0, 0, -1) * msec;
			}
			position.x = current.x;
		}
		else if (diffX != 0 && diffZ != 0) {
			if (diffX > 0) {
				//position -= Matrix4::Rotation(yaw, Vector3(0, 1, 0)) * Vector3(-1, 0, 0) * msec;
				if (diffZ > 0) {
					position -= Matrix4::Rotation(yaw, Vector3(0, 1, 0)) * Vector3(-1, 0, -1) * msec;
				}
				else {
					position += Matrix4::Rotation(yaw, Vector3(0, 1, 0)) * Vector3(1, 0, -1) * msec;
				}
			}
			else {
				//position += Matrix4::Rotation(yaw, Vector3(0, 1, 0)) * Vector3(-1, 0, 0) * msec;
				if (diffZ > 0) {
					position -= Matrix4::Rotation(yaw, Vector3(0, 1, 0)) * Vector3(1, 0, -1) * msec;
				}
				else {
					position += Matrix4::Rotation(yaw, Vector3(0, 1, 0)) * Vector3(-1, 0, -1) * msec;
				}
			}
		}
		if (nextPos.y - current.y != 0) {
			position.y += nextPos.y - current.y;
		}
		if (diffX != 0 && ((position.x > nextPos.x&& diffX > 0) || (position.x < nextPos.x && diffX < 0)))
			position.x = nextPos.x;
		if (diffZ != 0 && ((position.z > nextPos.z&& diffZ > 0) || (position.z < nextPos.z && diffZ < 0)))
			position.z = nextPos.z;
		float intpartPosX, intpartPosZ, intpartNextPosX, intpartNextPosZ, intpartYaw, intpartNextYaw;
		modf(position.x, &intpartPosX);
		modf(position.z, &intpartPosZ);
		modf(yaw, &intpartYaw);
		modf(nextPos.x, &intpartNextPosX);
		modf(nextPos.z, &intpartNextPosZ);
		modf(nextYaw, &intpartNextYaw);
		if (intpartPosX == intpartNextPosX && intpartPosZ == intpartNextPosZ && intpartYaw == intpartNextYaw) {
			if (!toNextPosition) {
				currentPosition--;
			}
			else {
				currentPosition++;
			}
			yaw = 0;
			cout << "currentPosition : " << currentPosition << endl;
			cout << "current yaw : " << nextYaw << endl;
		}
		
	}
	else { return; }
}
