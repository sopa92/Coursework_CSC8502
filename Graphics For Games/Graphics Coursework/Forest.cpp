#include "Forest.h"

Forest::Forest(int amount)
{
	for (int i = 0; i < amount; ++i) {
		Tree* createdTree = new Tree();
		cout << "Origin Tree " << i + 1 << " " << createdTree->GetTransform() << endl;
		createdTree->SetTransform(createdTree->GetTransform() * Matrix4::Translation(Vector3(0.5f * i / 18.f, 0, 0.5f * i / 18.f)));
		
		cout << "Tree " << i + 1 << " " << createdTree->GetTransform() << endl;
	}
}

Forest::~Forest(void)
{
}
