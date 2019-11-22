#include "Wall.h"

Mesh* Wall::cube = NULL;

Wall::Wall(bool hasWindow) {
	SceneNode* cube_wall = new SceneNode();
	AddChild(cube_wall);
	if (!hasWindow) {
		full_wall = new SceneNode(cube, Vector4(0, 1, 0, 1));
		full_wall->name = "full_wall";
		full_wall->SetModelScale(Vector3(1.0f, 1.0f, 0.02f));
		full_wall->SetTransform(Matrix4::Translation(Vector3(0, 0, 0)));
		cube_wall->AddChild(full_wall);
	}
	else {
		bottom_wall = new SceneNode(cube, Vector4(0, 1, 0, 1));
		bottom_wall->name = "bottom_wall";
		bottom_wall->SetModelScale(Vector3(0.7f, 0.3f, 0.02f));
		bottom_wall->SetTransform(Matrix4::Translation(Vector3(0, 0, 0)));
		cube_wall->AddChild(bottom_wall);

		left_wall = new SceneNode(cube, Vector4(0, 1, 0, 1));
		left_wall->name = "left_wall";
		left_wall->SetModelScale(Vector3(0.3f, 0.7f, 0.02f));
		left_wall->SetTransform(Matrix4::Translation(Vector3(0, 0.3f, 0)));
		cube_wall->AddChild(left_wall);

		top_wall = new SceneNode(cube, Vector4(0, 1, 0, 1));
		top_wall->name = "top_wall";
		top_wall->SetModelScale(Vector3(0.7f, 0.3f, 0.02f));
		top_wall->SetTransform(Matrix4::Translation(Vector3(0.3f, 0.7f, 0)));
		cube_wall->AddChild(top_wall);

		right_wall = new SceneNode(cube, Vector4(0, 1, 0, 1));
		right_wall->name = "right_wall";
		right_wall->SetModelScale(Vector3(0.3f, 0.7f, 0.02f));
		right_wall->SetTransform(Matrix4::Translation(Vector3(0.7f, 0, 0)));
		cube_wall->AddChild(right_wall);
	}
}

void Wall::Update(float msec)
{
}
