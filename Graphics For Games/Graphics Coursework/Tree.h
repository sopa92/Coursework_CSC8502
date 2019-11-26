#pragma once
#include "../../nclgl/SceneNode.h"
#include "../../nclgl/OBJMesh.h"

class Tree : public SceneNode {
public:
	Tree(void);
	~Tree(void) {}
	virtual void Update(float msec);

	static void CreateMesh(string meshType) {
			OBJMesh* shapeMesh = new OBJMesh();
			shapeMesh->LoadOBJMesh(MESHDIR"" + meshType + ".obj");
		if (meshType == "cylinder") {
			cylinder = shapeMesh;
			cylinder->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"trunk.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0));
		} 
		else if (meshType == "cone") {
			cone = shapeMesh;
			cone->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"pine-texture.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0));
		}
	}
	
	static void DeleteTree() {
		delete cylinder;
		delete cone;
	}
protected:
	static Mesh* cylinder;
	static Mesh* cone;
	SceneNode* leaves;
};

