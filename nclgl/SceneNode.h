#pragma once
#include "Matrix4.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Mesh.h"
#include <vector>

class SceneNode {
public:
	float GetBoundingRadius() const { return boundingRadius; }
	void SetBoundingRadius(float f) { boundingRadius = f; }

	float GetCameraDistance() const { return distanceFromCamera; }
	void SetCameraDistance(float f) { distanceFromCamera = f; }

	static bool CompareByCameraDistance(SceneNode* a, SceneNode* b) {
		return (a->distanceFromCamera <	b->distanceFromCamera) ? true : false;
	}

	SceneNode(Mesh* m = NULL, Vector4 colour = Vector4(1, 1, 1, 1));
	~SceneNode(void);

	void SetTransform(const Matrix4& matrix) { transform = matrix; }
	const Matrix4& GetTransform() const { return transform; }
	Matrix4 GetWorldTransform() const { return worldTransform; }

	Vector4 GetColour() const { return colour; }
	void SetColour(Vector4 c) { colour = c; }

	Vector3 GetModelScale() const { return modelScale; }
	void SetModelScale(Vector3 s) { modelScale = s; }

	Mesh* GetMesh() const { return mesh; }
	void SetMesh(Mesh* m) { mesh = m; }

	void SetTexture(GLuint tex) { mesh->SetTexture(tex); }
	GLuint GetTexture() { return mesh->GetTexture(); }

	void AddChild(SceneNode* s);
	void DeleteChildren();

	virtual void Update(float msec);
	virtual void Draw(const OGLRenderer& r);

	std::vector<SceneNode*>::iterator GetChildIteratorStart() {
		return children.begin();
	}

	std::vector<SceneNode*>::iterator GetChildIteratorEnd() {
		return children.end();
	}
	string name;
protected:
	SceneNode* parent;
	Mesh* mesh;
	Matrix4 worldTransform;
	Matrix4 transform;
	Vector3 modelScale;
	Vector4 colour;

	float distanceFromCamera;
	float boundingRadius;

	std::vector<SceneNode*> children;
};