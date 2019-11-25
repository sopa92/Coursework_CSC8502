#include "Tree.h"

Mesh* Tree::cylinder = NULL;
Mesh* Tree::cone = NULL;


Tree::Tree(void) {
	
	SceneNode* trunk = new SceneNode(cylinder, Vector4(1, 0, 0, 1));
	trunk->name = "trunk";
	trunk->SetModelScale(Vector3(0.3f, 1.0f, 0.3f));
	trunk->SetTransform(Matrix4::Translation(Vector3(0, 0, 0)));
	AddChild(trunk);
	
	for (int i = 0; i < 5; ++i) {
		leaves = new SceneNode(cone, Vector4(0, 1, 0, 1));
		leaves->name = "leave " + to_string(i);
		leaves->SetModelScale(trunk->GetModelScale() * Vector3(4 - (i * 0.5f), 0.6f, 4 - (i * 0.5f)));
		leaves->SetTransform(Matrix4::Translation(Vector3(0, 1.0f + (i * 0.4f), 0)));
		trunk->AddChild(leaves);
	}
}

void Tree::Update(float msec) {
	float itemHeight = transform.GetScalingVector().y;
	if (transform.GetPositionVector().y < 10.0f + itemHeight) {
		transform = transform * Matrix4::Translation(Vector3(0, 1 / (5* itemHeight *msec), 0));
	}
	int count = 0;
	for (auto& leave : children) {
		if (count % 2 == 0)
			leave->SetTransform(leave->GetTransform() * Matrix4::Rotation(1 / (1.8f * itemHeight * msec), Vector3(0, 1, 0)));
		else
			leave->SetTransform(leave->GetTransform() * Matrix4::Rotation((1 / (1.8f * itemHeight * msec))-360, Vector3(0, 1, 0)));
		count++;
	}
	SceneNode::Update(msec);
}
