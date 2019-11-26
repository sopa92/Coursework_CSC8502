#include "Renderer.h"

Renderer::Renderer(Window& parent) : OGLRenderer(parent) {

	Tree::CreateMesh("cylinder");
	Tree::CreateMesh("cone");
	House::CreateMesh("cube_floor");
	House::CreateMesh("cube_wall");
	House::CreateMesh("cube_roof");
	House::CreateMesh("cube_door");
	House::CreateMesh("triangle");
	House::CreateMesh("sphere");

	root = new SceneNode();
	root->name = "root";

	camera = new Camera();
	camera->SetPosition(Vector3(RAW_WIDTH * HEIGHTMAP_X / 1.7f, 180.0f, RAW_WIDTH * HEIGHTMAP_Z));

	heightMap = new HeightMap(TEXTUREDIR"/terrain.raw");
	quad = Mesh::GenerateQuad();
	quadWater = Mesh::GenerateQuad();

	light = new Light(Vector3((RAW_HEIGHT * HEIGHTMAP_X) / 1.5f, 400.0f, (RAW_HEIGHT * HEIGHTMAP_Z)/1.1f),
		Vector4(1.0f, 1.0f, 1.0f, 1), (RAW_WIDTH * HEIGHTMAP_X));

	reflectShader = new Shader(SHADERDIR"reflectVertex.glsl", SHADERDIR"reflectFragment.glsl");

	skyboxShader = new Shader(SHADERDIR"skyboxVertex.glsl", SHADERDIR"skyboxFragment.glsl");

	lightShader = new Shader(SHADERDIR"PerPixelVertex.glsl", SHADERDIR"PerPixelFragment.glsl");

	processShader = new Shader(SHADERDIR"TexturedVertex.glsl", SHADERDIR"processfrag.glsl");

	if (!skyboxShader->LinkProgram() || !reflectShader->LinkProgram()
		|| !lightShader->LinkProgram() || !processShader->LinkProgram()) {
		return;
	}
	//----------------------
	sceneShader = new Shader(SHADERDIR"shadowscenevert.glsl", SHADERDIR"shadowscenefrag.glsl");
	shadowShader = new Shader(SHADERDIR"shadowVert.glsl", SHADERDIR"shadowFrag.glsl");

	presentSceneShader = new Shader(SHADERDIR"TexturedVertex.glsl", SHADERDIR"TexturedFragment.glsl");

	if (!sceneShader->LinkProgram() || !shadowShader->LinkProgram() || !presentSceneShader->LinkProgram()) {
		return;
	}

	glGenTextures(1, &shadowTex);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

	glBindTexture(GL_TEXTURE_2D, 0);

	glGenFramebuffers(1, &shadowFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowTex, 0);
	glDrawBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//-----------------------
	quadWater->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"water.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	heightMap->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"GrassFrozen.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	heightMap->SetBumpMap(SOIL_load_OGL_texture(TEXTUREDIR"Barren RedsDOT3.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	cubeMap = SOIL_load_OGL_cubemap(TEXTUREDIR"iceflats_west.tga", TEXTUREDIR"iceflats_east.tga",
		TEXTUREDIR"iceflats_up.tga", TEXTUREDIR"iceflats_down.tga",
		TEXTUREDIR"iceflats_south.tga", TEXTUREDIR"iceflats_north.tga",
		SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, 0);

	if (!cubeMap || !quadWater->GetTexture() || !heightMap->GetTexture() || !heightMap->GetBumpMap()) {
		return;
	}

	SetTextureRepeating(quadWater->GetTexture(), true);
	SetTextureRepeating(heightMap->GetTexture(), true);
	SetTextureRepeating(heightMap->GetBumpMap(), true);

	glGenTextures(1, &bufferDepthTex);
	glBindTexture(GL_TEXTURE_2D, bufferDepthTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);

	for (int i = 0; i < 2; ++i) {
		glGenTextures(1, &bufferColourTex[i]);
		glBindTexture(GL_TEXTURE_2D, bufferColourTex[i]);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	}

	glGenFramebuffers(1, &bufferFBO); // We 'll render the scene into this
	glGenFramebuffers(1, &processFBO); // And do post processing in this

	glBindFramebuffer(GL_FRAMEBUFFER, bufferFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, bufferDepthTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, bufferDepthTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bufferColourTex[0], 0);
	// We can check FBO attachment success using this command !
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) !=
		GL_FRAMEBUFFER_COMPLETE || !bufferDepthTex || !bufferColourTex[0]) {
		return;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	CreateObjectsOnScene();

	init = true;
	waterRotate = waterAscent = 0.0f;
	projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
}

Renderer::~Renderer(void) {
	delete root;
	Tree::DeleteTree();
	House::DeleteHouse();
	delete camera;
	delete heightMap;
	delete quad;
	delete quadWater;
	delete reflectShader;
	delete skyboxShader;
	delete lightShader;
	delete light;
	delete sceneShader;
	delete shadowShader;
	delete processShader;
	glDeleteTextures(2, bufferColourTex);
	glDeleteTextures(1, &bufferDepthTex);
	glDeleteFramebuffers(1, &bufferFBO);
	glDeleteFramebuffers(1, &processFBO);
	currentShader = 0;
}

void Renderer::RenderScene() {
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);
	DrawShadowScene(); // First render pass ...

	viewMatrix = camera->BuildViewMatrix();

	glBindFramebuffer(GL_FRAMEBUFFER, bufferFBO);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	DrawSkybox();	
	DrawWater();
	DrawHeightmap();
	DrawCombinedScene();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	DrawPostProcess();
	PresentScene();
	SwapBuffers();
}

void Renderer::DrawSkybox() {
	glDepthMask(GL_FALSE); //we don't want the quad to fill the depth buffer, and cause our heightmap and water to be discarded
	SetCurrentShader(skyboxShader); //enable our skybox shader

	UpdateShaderMatrices();
	quad->Draw();

	glUseProgram(0); //unbind the skybox shader
	glDepthMask(GL_TRUE);
}

void Renderer::DrawHeightmap() {


	//SetCurrentShader(sceneShader);


	SetCurrentShader(sceneShader); //enable the per fragment lighting shader
	SetShaderLight(*light);		//set the shader light

	//set the shader uniform variables
	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1, (float*)&camera->GetPosition());
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "bumpTex"), 1);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "shadowTex"), 2);

	modelMatrix.ToIdentity();
	textureMatrix.ToIdentity();

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, shadowTex);

	UpdateShaderMatrices();

	heightMap->Draw();
	glUseProgram(0);

}

void Renderer::DrawWater() {

	SetCurrentShader(reflectShader);//enable the per fragment reflection shader
	SetShaderLight(*light);

	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1, (float*)&camera->GetPosition());
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 9);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "cubeTex"), 2);


	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);

	glActiveTexture(GL_TEXTURE9);
	glBindTexture(GL_TEXTURE_2D, quadWater->GetTexture());
	glActiveTexture(GL_TEXTURE0);

	float heightX = (RAW_WIDTH * HEIGHTMAP_X / 2.0f);	//holds x axis position of the centre of the heightmap
	float heightY = waterAscent;			//the water level
	float heightZ = (RAW_HEIGHT * HEIGHTMAP_Z / 2.0f);	//holds z axis position of the centre of the heightmap

	modelMatrix =
		Matrix4::Translation(Vector3(heightX, heightY, heightZ)) *
		Matrix4::Scale(Vector3(heightX, 1, heightZ)) *
		Matrix4::Rotation(90, Vector3(1.0f, 0.0f, 0.0f));

	//set the texture matrix in	this function, to make the texture coordinates repeat, and be rotated about the z-axis by the value waterRotate
	textureMatrix = Matrix4::Scale(Vector3(10.0f, 10.0f, 10.0f)) * Matrix4::Rotation(waterRotate, Vector3(0.0f, 0.0f, 1.0f));

	UpdateShaderMatrices();

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	quadWater->Draw();
	glDisable(GL_CULL_FACE);

	glUseProgram(0);
}

void Renderer::DrawItems() {

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	if (!camera->GetSplitScreen()) {
		DrawNode(root);
	}
	else {
		glViewport(0, 0, width / 2, height);
		DrawNode(root);

		glViewport(width / 2, 0, width / 2, height);
		DrawNode(root);
	}
	glDisable(GL_CULL_FACE);
	glUseProgram(0);
}

void Renderer::DrawNode(SceneNode* n) {
	if (n->GetMesh()) {
		Matrix4 transform = n->GetWorldTransform() * Matrix4::Scale(n->GetModelScale());
		Matrix4 tempMatrix = shadowMatrix * transform;
		glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "modelMatrix"), 1, false, (float*)&transform);
		glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "useTexture"), (int)n->GetMesh()->GetTexture());
		glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "shadowMatrix"), 1, false, *&tempMatrix.values);

		n->Draw(*this);
	}

	for (vector<SceneNode*>::const_iterator
		i = n->GetChildIteratorStart();
		i != n->GetChildIteratorEnd(); ++i) {
		DrawNode(*i);
	}

}

void Renderer::UpdateScene(float msec) {

	camera->UpdateCamera();

	viewMatrix = camera->BuildViewMatrix();
	root->Update(msec);
	waterRotate += msec / 5000.0f;
	if (waterAscent < 70 * HEIGHTMAP_Y / 3.0f)
		waterAscent += msec / 350.0f;
}

void Renderer::UpdateTextureMatrix(float value) {
	Matrix4 pushPos = Matrix4::Translation(Vector3(0.5f, 0.5f, 0));
	Matrix4 popPos = Matrix4::Translation(Vector3(-0.5f, -0.5f, 0));
	Matrix4 rotation = Matrix4::Rotation(value, Vector3(0, 0, 1));
	textureMatrix = pushPos * rotation * popPos;
}

void Renderer::ToggleFiltering(GLuint target) {
	filtering = !filtering;
	glBindTexture(GL_TEXTURE_2D, target);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering ? GL_LINEAR : GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtering ? GL_LINEAR : GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Renderer::CreateObjectsOnScene() {
	Vector3 forestPosition = Vector3(((RAW_WIDTH * HEIGHTMAP_X) / 1.7f), -40.0f, ((RAW_WIDTH * HEIGHTMAP_X) / 1.17f));
	CreateForest(5, forestPosition, 30, 0);
	forestPosition = Vector3(((RAW_WIDTH * HEIGHTMAP_X) / 1.9f), -50.0f, (((RAW_WIDTH * HEIGHTMAP_X) / 1.17f)) - 370);
	CreateForest(6, forestPosition, 30, 10.0f);
	forestPosition = Vector3(((RAW_WIDTH * HEIGHTMAP_X) / 1.9f) - 200, -50.0f, (((RAW_WIDTH * HEIGHTMAP_X) / 1.17f)) - 190);
	CreateForest(4, forestPosition, 25, 0);
	forestPosition += Vector3(-400.0f, -20.0f, 20.0f);
	CreateForest(4, forestPosition, 30, -10.0f);

	Vector3 villagePosition = Vector3(((RAW_WIDTH * HEIGHTMAP_X) / 2.5f), -50, ((RAW_WIDTH * HEIGHTMAP_X) / 1.7f));
	CreateVillage(1, villagePosition, 20, 45);
	forestPosition = Vector3(((RAW_WIDTH * HEIGHTMAP_X) / 2.5f) - 300, -30, ((RAW_WIDTH * HEIGHTMAP_X) / 1.7f) + 200);
	CreateForest(5, forestPosition, 30, 10.0f);
}

void Renderer::CreateForest(int amountTrees, Vector3 treeTransform, float scale, float degrees)
{
	SceneNode* forest = new SceneNode();
	forest->name = "forest";
	if (amountTrees > 4) {
		int halfTrees = amountTrees / 2;
		int restTrees = amountTrees % 2;
		for (int i = 0; i < halfTrees; ++i) {
			Tree* newlyCreatedTree = new Tree();
			Matrix4 translationMatr = Matrix4::Translation(treeTransform + Vector3(scale + i * 3.0f * scale, 0.0f, 1));
			Matrix4 scalingMatr = Matrix4::Scale(Vector3(scale, scale, scale));
			newlyCreatedTree->SetTransform(newlyCreatedTree->GetTransform() * translationMatr * scalingMatr);
			forest->AddChild(newlyCreatedTree);
		}
		for (int i = 0; i < halfTrees + restTrees; ++i) {
			Tree* newlyCreatedTree = new Tree();
			Matrix4 translationMatr = Matrix4::Translation(treeTransform + Vector3(i * 3.0f * scale, 0.0f, -4 * scale));
			Matrix4 scalingMatr = Matrix4::Scale(Vector3(scale, scale, scale));
			newlyCreatedTree->SetTransform(newlyCreatedTree->GetTransform() * translationMatr * scalingMatr);
			forest->AddChild(newlyCreatedTree);
		}
	}
	else {
		for (int i = 0; i < amountTrees; ++i) {
			Tree* newlyCreatedTree = new Tree();
			Matrix4 translationMatr = Matrix4::Translation(treeTransform + Vector3(i * 3.0f * scale, 0.0f, (i % 2) * 3.0f * scale));
			Matrix4 scalingMatr = Matrix4::Scale(Vector3(scale, scale, scale));
			newlyCreatedTree->SetTransform(newlyCreatedTree->GetTransform() * translationMatr * scalingMatr);
			forest->AddChild(newlyCreatedTree);
		}
	}

	Matrix4 rotationMatr = Matrix4::Rotation(degrees, Vector3(0, 1, 0));
	forest->SetTransform(forest->GetTransform() * rotationMatr);
	root->AddChild(forest);
}

void Renderer::CreateVillage(int amountHouses, Vector3 houseTransform, float scale, float degrees)
{
	SceneNode* village = new SceneNode();
	Matrix4 rotationMatr = Matrix4::Rotation(degrees, Vector3(0, 1, 0));
	for (int i = 0; i < amountHouses; ++i) {
		House* newlyCreatedHouse = new House();
		Matrix4 translationMatr = Matrix4::Translation(houseTransform + Vector3(i * 3.0f * scale, 0.0f, (i % 2) * 3.0f * scale));
		Matrix4 scalingMatr = Matrix4::Scale(Vector3(scale, scale, scale));
		newlyCreatedHouse->SetTransform(newlyCreatedHouse->GetTransform() * translationMatr * scalingMatr);
		if (amountHouses == 1)
			newlyCreatedHouse->SetTransform(newlyCreatedHouse->GetTransform() * rotationMatr);
		village->AddChild(newlyCreatedHouse);
	}
	if (amountHouses > 1)
		village->SetTransform(village->GetWorldTransform() * rotationMatr);
	root->AddChild(village);
}


//----------------------------

void Renderer::DrawShadowScene() {
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);

	glClear(GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, 1024, 1024);

	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	SetCurrentShader(shadowShader);

	viewMatrix = Matrix4::BuildViewMatrix(light->GetPosition(), Vector3(0, 0, 0));
	shadowMatrix = biasMatrix * (projMatrix * viewMatrix);

	UpdateShaderMatrices();
	DrawHeightmap();
	SetCurrentShader(shadowShader);
	UpdateShaderMatrices();
	DrawItems();

	glUseProgram(0);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glViewport(0, 0, width, height);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::DrawCombinedScene() {
	SetCurrentShader(sceneShader);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "bumpTex"), 1);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "shadowTex"), 2);

	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1, (float*)&camera->GetPosition());

	SetShaderLight(*light);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, shadowTex);

	viewMatrix = camera->BuildViewMatrix();
	UpdateShaderMatrices();

	DrawItems();

	glUseProgram(0);
}

//----------------------------
void Renderer::DrawPostProcess() {
	glBindFramebuffer(GL_FRAMEBUFFER, processFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bufferColourTex[1], 0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	SetCurrentShader(processShader);
	projMatrix = Matrix4::Orthographic(-1, 1, 1, -1, -1, 1);
	viewMatrix.ToIdentity();
	textureMatrix.ToIdentity();
	modelMatrix.ToIdentity();
	UpdateShaderMatrices();

	glDisable(GL_DEPTH_TEST);

	glUniform2f(glGetUniformLocation(currentShader->GetProgram(), "pixelSize"), 1.0f / width, 1.0f / height);

	int blurPasses = 1;
	for (int i = 0; i < blurPasses; ++i) {
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bufferColourTex[1], 0);
		glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "isVertical"), 0);

		quad->SetTexture(bufferColourTex[0]);
		quad->Draw();
		// Now to swap the colour buffers , and do the second blur pass
		glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "isVertical"), 1);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bufferColourTex[0], 0);

		quad->SetTexture(bufferColourTex[1]);
		quad->Draw();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(0);

	glEnable(GL_DEPTH_TEST);
}

void Renderer::PresentScene() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	SetCurrentShader(presentSceneShader);
	projMatrix = Matrix4::Orthographic(-1, 1, 1, -1, -1, 1);
	viewMatrix.ToIdentity();
	modelMatrix.ToIdentity();
	UpdateShaderMatrices();
	quad->SetTexture(bufferColourTex[0]);
	quad->Draw();
	glUseProgram(0);
}
