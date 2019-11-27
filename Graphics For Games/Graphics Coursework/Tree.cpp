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
	float seconds = msec / 5000.0f;
	float itemHeight = transform.GetScalingVector().y;
	if (transform.GetPositionVector().y < itemHeight*1.3f) {
		transform = transform * Matrix4::Translation(Vector3(0, 1.0f * seconds, 0));
	}
	int count = 0;
	for (auto& childNode : children) {
		auto leaves = childNode->GetChildren();
		for (auto& leaf : leaves) {
			if (count % 2 == 0)
				leaf->SetTransform(leaf->GetTransform() * Matrix4::Rotation(seconds * 10, Vector3(0, 1, 0)));
			else
				leaf->SetTransform(leaf->GetTransform() * Matrix4::Rotation(-seconds * 10, Vector3(0, 1, 0)));
			count++;
		}
	}
	SceneNode::Update(msec);
}
