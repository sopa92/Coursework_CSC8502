#pragma once
#include "../../nclgl/OGLRenderer.h"
#include "../../nclgl/Camera.h"
#include "../../nclgl/SceneNode.h"
#include "../../nclgl/HeightMap.h"
#include "Tree.h"

class Renderer : public OGLRenderer{
public:
	Renderer(Window& parent);
	virtual ~Renderer(void);
	virtual void RenderScene();

	void DrawSkybox();

	void DrawHeightmap();

	void DrawWater();

	void DrawItems();

	void SwitchToPerspective();
	void SwitchToOrthographic();

	inline void SetScale(float s) { scale = s; }
	inline void SetRotation(float r) { rotation = r; }
	inline void SetPosition(Vector3 p) { position = p; }

	virtual void UpdateScene(float msec);
	void UpdateTextureMatrix(float rotation);
	void ToggleFiltering(GLuint target);

protected:
	void DrawNode(SceneNode* n);
	void CreateForest(int amountTrees, Vector3 transform, float scale, float degrees);
	//---------------
	void DrawShadowScene(); 
	void DrawCombinedScene(); 
	Shader* sceneShader;
	Shader* shadowShader;
	//---------------
	GLuint shadowTex;
	GLuint shadowFBO;

	Mesh* quad;
	Mesh* cube;

	float scale;
	float rotation;
	Vector3 position;

	SceneNode* root;
	Camera* camera;

	bool filtering;
	bool repeating;

	Shader* lightShader;
	Shader* reflectShader;
	Shader* skyboxShader;

	HeightMap* heightMap;
	Light* light;
	GLuint cubeMap;
	float waterRotate;
	bool moveCameraManually=false;
};

