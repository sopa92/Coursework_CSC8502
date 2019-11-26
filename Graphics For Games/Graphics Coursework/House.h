#pragma once
#include "../../nclgl/SceneNode.h"
#include "../../nclgl/OBJMesh.h"
#include "Wall.h"

enum MeshType { Wall, Floor, Roof, Door, Triangle, Sphere };

class House : public SceneNode {
public:
	House(void);
	~House() {}
	virtual void Update(float msec);

	static void CreateMesh(string meshType) {
		if (meshType == "triangle") {
			triangle = Mesh::GenerateTriangle();
			triangle->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"wooden_wall.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0));
		}
		else {
			OBJMesh* shapeMesh = new OBJMesh();
			if (meshType == "cube_floor") {
				shapeMesh->LoadOBJMesh(MESHDIR"cube.obj");
				cube_floor = shapeMesh;
				cube_floor->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"wood_floor.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0));
			}
			else if (meshType == "cube_wall") {
				shapeMesh->LoadOBJMesh(MESHDIR"cube.obj");
				cube_wall = shapeMesh;
				cube_wall->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"wooden_wall.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0));
			}
			else if (meshType == "cube_roof") {
				shapeMesh->LoadOBJMesh(MESHDIR"cube.obj");
				cube_roof = shapeMesh;
				cube_roof->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"snowy-roof.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0));
			}
			else if (meshType == "cube_door") {
				shapeMesh->LoadOBJMesh(MESHDIR"cube.obj");
				cube_door = shapeMesh;
				cube_door->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"door.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0));
			}
			else if (meshType == "sphere") {
				shapeMesh->LoadOBJMesh(MESHDIR"sphere.obj");
				sphere = shapeMesh;
				sphere->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"bronze.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, 0));
			}
		}
	}

	static void DeleteHouse() {
		delete cube_floor;
		delete cube_wall;
		delete cube_roof;
		delete cube_door;
		delete triangle;
		delete sphere;
	}

	Mesh* GetMesh(MeshType type){
		switch (type)
		{
		case Floor:
			return cube_floor;
		case Wall:
			return cube_wall;
		case Door:
			return cube_door;
		case Roof:
			return cube_roof;
		case Triangle:
			return triangle;
		case Sphere:
			return sphere;
		default:
			return nullptr;
		}
	}

protected:
	static Mesh* cube_floor;
	static Mesh* cube_wall;
	static Mesh* cube_roof;
	static Mesh* cube_door;
	static Mesh* triangle;
	static Mesh* sphere;
	SceneNode* left_roof;
	SceneNode* right_roof;
	SceneNode* front_roof;
	SceneNode* back_roof;
	SceneNode* window;
	SceneNode* front_wall;
	SceneNode* left_wall;
	SceneNode* right_wall;
	SceneNode* back_wall;
	SceneNode* door;
	SceneNode* door_knob;
	SceneNode* balcony;
	SceneNode* door_up;
	SceneNode* door_knob_up;
	SceneNode* balcony_holder_base_left; 
	SceneNode* balcony_holder_base_right;
	SceneNode* balcony_holder_base_center;
	SceneNode* balcony_holder;
	SceneNode* balcony_holder_left;
	SceneNode* balcony_holder_right;

	void SetRepeating(Mesh* mesh);
};

