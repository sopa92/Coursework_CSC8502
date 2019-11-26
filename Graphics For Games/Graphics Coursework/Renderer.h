#pragma once
#include "../../nclgl/OGLRenderer.h"
#include "../../nclgl/Camera.h"
#include "../../nclgl/SceneNode.h"
#include "../../nclgl/HeightMap.h"
#include "Tree.h"
#include "House.h"

class Renderer : public OGLRenderer{
public:
	Renderer(Window& parent);
	virtual ~Renderer(void);
	virtual void RenderScene();

	void DrawSkybox();

	void DrawHeightmap();

	void DrawWater();

	void DrawItems();
	
	inline void SetScale(float s) { scale = s; }
	inline void SetRotation(float r) { rotation = r; }
	inline void SetPosition(Vector3 p) { position = p; }

	virtual void UpdateScene(float msec);
	void UpdateTextureMatrix(float rotation);
	void ToggleFiltering(GLuint target);
	
protected:
	void DrawNode(SceneNode* n);
	void CreateForest(int amountTrees, Vector3 transform, float scale, float degrees);
	void CreateVillage(int amountHouses, Vector3 houseTransform, float scale, float degrees);
	void CreateObjectsOnScene();
	GLuint shadowTex;
	GLuint shadowFBO;

	Mesh* quad;
	Mesh* quadWater;

	float scale;
	float rotation;
	Vector3 position;

	SceneNode* root;
	Camera* camera;

	bool filtering;
	bool repeating;

	
	void DrawShadowScene();
	void DrawCombinedScene();
	Shader* sceneShader;
	Shader* shadowShader;

	//----------------PP
	void PresentScene();
	void DrawPostProcess();
	Shader* processShader;
	GLuint bufferFBO;
	GLuint processFBO;
	GLuint bufferColourTex[2];
	GLuint bufferDepthTex;
	//----------------PP

	Shader* lightShader;
	Shader* reflectShader;
	Shader* skyboxShader;
	Shader* presentSceneShader;

	HeightMap* heightMap;
	Light* light;
	Light* pointLight;
	GLuint cubeMap;
	float waterRotate;
	float waterAscent;
	bool moveCameraManually = false;
};

