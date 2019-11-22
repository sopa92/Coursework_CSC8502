#include "House.h"

Mesh* House::cube_floor = NULL;
Mesh* House::cube_wall = NULL;
Mesh* House::cube_roof = NULL;
Mesh* House::cube_door = NULL;
Mesh* House::triangle = NULL;
Mesh* House::sphere = NULL;

House::House(void) {
	
	SceneNode* houseBase = new SceneNode(cube_floor, Vector4(0, 0, 0, 1));
	houseBase->name = "houseBase";
	houseBase->SetModelScale(Vector3(5.5f, 1.5f, 4.0f));
	houseBase->SetTransform(Matrix4::Translation(Vector3(0, 0, 0)));
	AddChild(houseBase);

	back_wall = new SceneNode(cube_wall, Vector4(0, 1, 0, 1));
	back_wall->name = "back_wall";
	back_wall->SetModelScale(houseBase->GetModelScale() * Vector3(0.8f, 2.5f, 0.04f));
	back_wall->SetTransform(Matrix4::Translation(Vector3(0, 1.0f, -3)));
	houseBase->AddChild(back_wall);

	front_wall = new SceneNode(cube_wall, Vector4(0, 1, 0, 1));
	front_wall->name = "front_wall";
	front_wall->SetModelScale(houseBase->GetModelScale() * Vector3(0.8f, 2.5f, 0.04f));
	front_wall->SetTransform(Matrix4::Translation(Vector3(0, 1.0f, 2.0f)));
	houseBase->AddChild(front_wall);

	left_wall = new SceneNode(cube_wall, Vector4(0, 1, 0, 1));
	left_wall->name = "back_wall";
	left_wall->SetModelScale(houseBase->GetModelScale() * Vector3(0.04f, 2.5f, 0.665f));
	left_wall->SetTransform(Matrix4::Translation(Vector3(-4.18f, 1.0f, -0.496f)));
	houseBase->AddChild(left_wall);

	right_wall = new SceneNode(cube_wall, Vector4(0, 1, 0, 1));
	right_wall->name = "front_wall";
	right_wall->SetModelScale(houseBase->GetModelScale() * Vector3(0.04f, 2.5f, 0.665f));
	right_wall->SetTransform(Matrix4::Translation(Vector3(4.18f, 1.0f, -0.496f)));
	houseBase->AddChild(right_wall);

	door = new SceneNode(cube_door, Vector4(0, 1, 0, 1));
	door->name = "door";
	door->SetModelScale(houseBase->GetModelScale() * Vector3(0.12f, 1.0f, 0.05f));
	door->SetTransform(Matrix4::Translation(Vector3(1.0f, 1.5f, 0.0f)));
	front_wall->AddChild(door);

	door_knob = new SceneNode(sphere, Vector4(0, 1, 0, 1));
	door_knob->name = "door_knob";
	door_knob->SetModelScale(houseBase->GetModelScale() * Vector3(0.015f, 0.05f, 0.015f));
	door_knob->SetTransform(door->GetTransform() * Matrix4::Translation(Vector3(-0.6f, 0.3f, 0.2f)));
	door->AddChild(door_knob);

	SceneNode* roof = new SceneNode();
	roof->SetTransform(Matrix4::Translation(Vector3(0, 2, 0)));
	houseBase->AddChild(roof);

	left_roof = new SceneNode(cube_roof, Vector4(0, 1, 0, 1));
	left_roof->name = "left_roof";
	left_roof->SetModelScale(roof->GetModelScale() * Vector3(0.1f, 3.0f, 3.0f));
	left_roof->SetTransform(Matrix4::Translation(Vector3(-5.13f, 6.0f, -0.496f)) * Matrix4::Rotation(60.0f, Vector3(0,0,-1)));
	roof->AddChild(left_roof);

	right_roof = new SceneNode(cube_roof, Vector4(0, 1, 0, 1));
	right_roof->name = "right_roof";
	right_roof->SetModelScale(roof->GetModelScale() * Vector3(0.1f, 3.0f, 3.0f));
	right_roof->SetTransform(Matrix4::Translation(Vector3(5.13f, 6.0f, -0.496f)) * Matrix4::Rotation(-60.0f, Vector3(0, 0, -1)));
	roof->AddChild(right_roof);
	
	front_roof = new SceneNode(triangle, Vector4(0, 1, 0, 1));
	front_roof->name = "front_roof";
	front_roof->SetModelScale(roof->GetModelScale() * Vector3(5.0f, 1.4f, 0));
	front_roof->SetTransform(Matrix4::Translation(Vector3(0.0f, 7.6f, 2.15f)));
	roof->AddChild(front_roof);

	back_roof = new SceneNode(triangle, Vector4(0, 1, 0, 1));
	back_roof->name = "back_roof";
	back_roof->SetModelScale(roof->GetModelScale() * Vector3(5.0f, 1.4f, 0));
	back_roof->SetTransform(Matrix4::Translation(Vector3(0.0f, 7.6f, -3.15f)));
	roof->AddChild(back_roof);

	balcony = new SceneNode(cube_floor, Vector4(0, 0, 0, 1));
	balcony->name = "balcony";
	balcony->SetModelScale(houseBase->GetModelScale() * Vector3(0.75f, 0.05f, 0.2f));
	balcony->SetTransform(Matrix4::Translation(Vector3(0, 5.0f, 0.8f)));
	front_wall->AddChild(balcony);

	door_up = new SceneNode(cube_door, Vector4(0, 1, 0, 1));
	door_up->name = "door_up";
	door_up->SetModelScale(houseBase->GetModelScale() * Vector3(0.12f, 0.8f, 0.05f));
	door_up->SetTransform(Matrix4::Translation(Vector3(-1.5f, 5.0f, 0.0f)));
	front_wall->AddChild(door_up);

	door_knob_up = new SceneNode(sphere, Vector4(0, 1, 0, 1));
	door_knob_up->name = "door_knob_up";
	door_knob_up->SetModelScale(houseBase->GetModelScale() * Vector3(0.015f, 0.05f, 0.015f));
	door_knob_up->SetTransform(door_up->GetTransform() * Matrix4::Translation(Vector3(1.8f, -3.7f, 0.2f)));
	door_up->AddChild(door_knob_up);

	balcony_holder_base_left = new SceneNode(cube_floor, Vector4(0, 0, 0, 1));
	balcony_holder_base_left->name = "balcony_holder_base_left";
	balcony_holder_base_left->SetModelScale(houseBase->GetModelScale() * Vector3(0.01f, 0.3f, 0.02f));
	balcony_holder_base_left->SetTransform(houseBase->GetTransform() * Matrix4::Translation(Vector3(-4.1f, 0, 0.7f)));
	balcony->AddChild(balcony_holder_base_left);

	balcony_holder_base_right = new SceneNode(cube_floor, Vector4(0, 0, 0, 1));
	balcony_holder_base_right->name = "balcony_holder_base_right";
	balcony_holder_base_right->SetModelScale(houseBase->GetModelScale() * Vector3(0.01f, 0.3f, 0.02f));
	balcony_holder_base_right->SetTransform(houseBase->GetTransform() * Matrix4::Translation(Vector3(4.1f, 0, 0.7f)));
	balcony->AddChild(balcony_holder_base_right);

	balcony_holder_base_center = new SceneNode(cube_floor, Vector4(0, 0, 0, 1));
	balcony_holder_base_center->name = "balcony_holder_base_center";
	balcony_holder_base_center->SetModelScale(houseBase->GetModelScale()* Vector3(0.01f, 0.3f, 0.02f));
	balcony_holder_base_center->SetTransform(houseBase->GetTransform()* Matrix4::Translation(Vector3(0.0f, 0, 0.7f)));
	balcony->AddChild(balcony_holder_base_center);

	balcony_holder = new SceneNode(cube_floor, Vector4(0, 0, 0, 1));
	balcony_holder->name = "balcony_holder";
	balcony_holder->SetModelScale(houseBase->GetModelScale()* Vector3(0.755f, 0.02f, 0.02f));
	balcony_holder->SetTransform(houseBase->GetTransform()* Matrix4::Translation(Vector3(0.0f, 0.85f, 0.7f)));
	balcony->AddChild(balcony_holder);


	balcony_holder_left = new SceneNode(cube_floor, Vector4(0, 0, 0, 1));
	balcony_holder_left->name = "balcony_holder_left ";
	balcony_holder_left->SetModelScale(houseBase->GetModelScale()* Vector3(0.01f, 0.02f, 0.193f));
	balcony_holder_left->SetTransform(houseBase->GetTransform()* Matrix4::Translation(Vector3(-4.1f, 0.85f, 0)));
	balcony->AddChild(balcony_holder_left);

	balcony_holder_right = new SceneNode(cube_floor, Vector4(0, 0, 0, 1));
	balcony_holder_right->name = "balcony_holder_right";
	balcony_holder_right->SetModelScale(houseBase->GetModelScale()* Vector3(0.01f, 0.02f, 0.193f));
	balcony_holder_right->SetTransform(houseBase->GetTransform()* Matrix4::Translation(Vector3(4.1f, 0.85f, 0)));
	balcony->AddChild(balcony_holder_right);
}

void House::Update(float msec) {
	//float itemHeight = transform.GetScalingVector().y;

	//if (transform.GetPositionVector().y < 5.0f + itemHeight) {
	//	transform = transform * Matrix4::Translation(Vector3(0, 1 / (5 * itemHeight * msec), 0)) * Matrix4::Rotation(45, Vector3(0, 1 / (50 * msec), 0));
	//	//transform = transform );
	//}
	
	SceneNode::Update(msec);
}