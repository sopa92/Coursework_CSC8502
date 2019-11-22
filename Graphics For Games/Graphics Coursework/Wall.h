#pragma once
#include "../../nclgl/SceneNode.h"
#include "../../nclgl/OBJMesh.h"

class Wall : public SceneNode {
public:
	Wall(bool hasWindow);
	~Wall(void) {}
	virtual void Update(float msec);

	static void CreateCube() {
		OBJMesh* shapeMesh = new OBJMesh();
		shapeMesh->LoadOBJMesh(MESHDIR"cube.obj");
		cube = shapeMesh;
		cube->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"wooden_wall3.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0));

	}

	static void DeleteWall() {
		delete cube;
	}
protected:
	static Mesh* cube;
	SceneNode* full_wall;
	SceneNode* top_wall;
	SceneNode* left_wall;
	SceneNode* bottom_wall;
	SceneNode* right_wall;

};
