#include "SceneNode.h"

SceneNode::SceneNode(Mesh* mesh, Vector4 colour) {
	this->mesh = mesh;
	this->colour = colour;
	parent = NULL;
	modelScale = Vector3(1, 1, 1);
	boundingRadius = 1.0f;
	distanceFromCamera = 0.0f;
}

SceneNode::~SceneNode(void) {
	for (unsigned int i = 0; i < children.size(); ++i) {
		delete children[i];
	}
}

void SceneNode::AddChild(SceneNode* s) {
	children.push_back(s);
	s->parent = this;
}

//void SceneNode::DeleteChild(SceneNode* s) {
//	for (vector<SceneNode*>::iterator i = s->children.begin();	i != s->children.end(); ++i) {
//		DeleteChild(*i);
//	}
//	vector<SceneNode*>::iterator it = find(children.begin(), children.end(), s);
//	if (it != children.end()) {
//		children.erase(children.begin() + distance(children.begin(), it));
//	}
//}

void SceneNode::DeleteChildren() {
	if (children.size() > 0) {
		vector<SceneNode*> childrenCopy = children;
		for (vector<SceneNode*>::iterator i = childrenCopy.begin(); i != childrenCopy.end(); ++i) {
			vector<SceneNode*> descendants = (*i)->children;
			if (descendants.size() > 0) {
				for (vector<SceneNode*>::iterator it = descendants.begin(); i != descendants.end(); ++i) {
					(*i)->DeleteChildren();
				}
			}
		}
		children.erase(children.begin(), children.end());
	}
}

void SceneNode::Update(float msec) {
	if (parent) { // This node has a parent ...
		worldTransform = parent->worldTransform * transform;
	}
	else { // Root node , world transform is local transform !
		worldTransform = transform;
	}
	for (vector<SceneNode*>::iterator i = children.begin();	i != children.end(); ++i) {
		(*i)->Update(msec);
	}
}

void SceneNode::Draw(const OGLRenderer& r) {
	if (mesh) { mesh->Draw(); }
}
