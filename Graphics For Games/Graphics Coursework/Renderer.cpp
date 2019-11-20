#include "Renderer.h"

Renderer::Renderer(Window& parent) : OGLRenderer(parent) {

	Tree::CreateMesh("cylinder");
	Tree::CreateMesh("cone");
	root = new SceneNode();
	root->name = "root";

	camera = new Camera();
	heightMap = new HeightMap(TEXTUREDIR"/terrain.raw");
	quad = Mesh::GenerateQuad();

	camera->SetInitialPosition(Vector3(RAW_WIDTH * HEIGHTMAP_X / 1.7f, 100.0f, RAW_WIDTH * HEIGHTMAP_Z));
	light = new Light(Vector3((RAW_HEIGHT * HEIGHTMAP_X / 2.0f), 500.0f, (RAW_HEIGHT * HEIGHTMAP_Z)/1.5f),
		Vector4(0.9f, 0.9f, 1.0f, 1), (RAW_WIDTH * HEIGHTMAP_X) / 1.5f);


	//currentShader = new Shader(SHADERDIR"SceneVertex.glsl", SHADERDIR"SceneFragment.glsl");

	reflectShader = new Shader(SHADERDIR"PerPixelVertex.glsl", SHADERDIR"reflectFragment.glsl");

	skyboxShader = new Shader(SHADERDIR"skyboxVertex.glsl", SHADERDIR"skyboxFragment.glsl");

	lightShader = new Shader(SHADERDIR"PerPixelVertex.glsl", SHADERDIR"PerPixelFragment.glsl");

	if ( !skyboxShader->LinkProgram() || 
		!reflectShader->LinkProgram() || !lightShader->LinkProgram()) { //!currentShader->LinkProgram() ||
		return;
	}
	//----------------------
	sceneShader = new Shader(SHADERDIR"shadowscenevert.glsl", SHADERDIR"shadowscenefrag.glsl");
	shadowShader = new Shader(SHADERDIR"shadowVert.glsl", SHADERDIR"shadowFrag.glsl");

	if (!sceneShader->LinkProgram() || !shadowShader->LinkProgram()) {
		return;
	}

	glGenTextures(1, &shadowTex);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		2048, 2048, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

	glBindTexture(GL_TEXTURE_2D, 0);

	glGenFramebuffers(1, &shadowFBO);

	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowTex, 0);
	glDrawBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//-----------------------
	quad->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"water.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	heightMap->SetTexture(SOIL_load_OGL_texture(TEXTUREDIR"GrassFrozen1.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	heightMap->SetBumpMap(SOIL_load_OGL_texture(TEXTUREDIR"heightmap_test.JPG", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS));

	cubeMap = SOIL_load_OGL_cubemap(TEXTUREDIR"iceflats_west.tga", TEXTUREDIR"iceflats_east.tga",
		TEXTUREDIR"iceflats_up.tga", TEXTUREDIR"iceflats_down.tga",
		TEXTUREDIR"iceflats_south.tga", TEXTUREDIR"iceflats_north.tga",
		SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, 0);
	
	if (!cubeMap || !quad->GetTexture() || !heightMap->GetTexture() || !heightMap->GetBumpMap()) {
		return;
	}

	SetTextureRepeating(quad->GetTexture(), true);
	filtering = false;
	ToggleFiltering(quad->GetTexture());
	SetTextureRepeating(heightMap->GetTexture(), true);
	SetTextureRepeating(heightMap->GetBumpMap(), true);

	Vector3 forestTransform = Vector3(((RAW_WIDTH * HEIGHTMAP_X) / 1.7f), -40.0f, ((RAW_WIDTH * HEIGHTMAP_X) / 1.17f));
	CreateForest(5, forestTransform, 20, 0.0f);

	forestTransform += Vector3(30.0f, -20.0f, -300.0f);
	CreateForest(6, forestTransform, 20, 5.0f);
	forestTransform += Vector3(10.0f, -20.0f, -70.0f);
	CreateForest(4, forestTransform, 20, 5.0f);
	forestTransform += Vector3(-400.0f, -20.0f, 20.0f);
	CreateForest(4, forestTransform, 20, -10.0f);

	init = true;
	waterRotate = 0.0f;
	SwitchToPerspective();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
}

Renderer::~Renderer(void) {
	delete root;
	Tree::DeleteTree();	
	delete camera;
	delete heightMap;
	delete quad;
	delete reflectShader;
	delete skyboxShader;
	delete lightShader;
	delete light;
	delete sceneShader;
	delete shadowShader;
	currentShader = 0;
}

void Renderer::RenderScene() {
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	DrawSkybox();
	DrawHeightmap();
	DrawWater();
	DrawShadowScene(); // First render pass ...
	DrawCombinedScene(); // Second render pass ...
	
	SwapBuffers();
}

void Renderer::DrawSkybox() {
	glDepthMask(GL_FALSE); //we don't want the quad to fill the depth buer, and cause our heightmap and water to be discarded
	SetCurrentShader(skyboxShader); //enable our skybox shader

	UpdateShaderMatrices();
	quad->Draw();

	glUseProgram(0); //unbind the skybox shader
	glDepthMask(GL_TRUE);
}

void Renderer::DrawHeightmap() {

	SetCurrentShader(lightShader); //enable the per fragment lighting shader
	SetShaderLight(*light);		//set the shader light

	//set the shader uniform variables
	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1, (float*)&camera->GetPosition());
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "bumpTex"), 1);

	modelMatrix.ToIdentity();
	textureMatrix.ToIdentity();

	UpdateShaderMatrices();

	heightMap->Draw();
	glUseProgram(0);
}

void Renderer::DrawWater() {

	SetCurrentShader(reflectShader);//enable the per fragment reflection shader
	SetShaderLight(*light);

	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1, (float*)&camera->GetPosition());
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "cubeTex"), 2); 
	
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);

	float heightX = (RAW_WIDTH * HEIGHTMAP_X / 2.0f);	//holds x axis position of the centre of the heightmap
	float heightY = 70 * HEIGHTMAP_Y / 3.0f;			//the water level
	float heightZ = (RAW_HEIGHT * HEIGHTMAP_Z / 2.0f);	//holds z axis position of the centre of the heightmap

	modelMatrix =
		Matrix4::Translation(Vector3(heightX, heightY, heightZ)) *
		Matrix4::Scale(Vector3(heightX, 1, heightZ)) *
		Matrix4::Rotation(90, Vector3(1.0f, 0.0f, 0.0f));

	//set the texture matrix in	this function, to make the texture coordinates repeat, and be rotated about the z-axis by the value waterRotate
	textureMatrix = Matrix4::Scale(Vector3(10.0f, 10.0f, 10.0f)) * Matrix4::Rotation(waterRotate, Vector3(0.0f, 0.0f, 1.0f));

	UpdateShaderMatrices();

	quad->Draw();

	glUseProgram(0);
}

void Renderer::DrawItems() {

	//SetCurrentShader(lightShader); //enable the per fragment lighting shader
	//SetShaderLight(*light);		//set the shader light
	//glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1, (float*)&camera->GetPosition());
	//glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 0);
	//glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "cubeTex"), 2);
	modelMatrix.ToIdentity();

	Matrix4 tempMatrix = textureMatrix * modelMatrix;

	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "textureMatrix"), 1, false, *&tempMatrix.values);

	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "modelMatrix"), 1, false, *&modelMatrix.values);

	DrawNode(root);

	glUseProgram(0);
}

void Renderer::SwitchToPerspective(){
	projMatrix = Matrix4::Perspective(1.0f, 15000.0f, (float)width / (float)height, 45.0f);
}

void Renderer::SwitchToOrthographic() {
	projMatrix = Matrix4::Orthographic(-1.0f, 15000.0f, width / 2.0f, -width / 2.0f, height / 2.0f, -height / 2.0f);
}

void Renderer::UpdateScene(float msec) {
	
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_M)) {
		moveCameraManually = true;
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_P)) {
		cout << camera->GetPosition() << endl;
	}

	if (moveCameraManually)
		camera->UpdateCameraManually();
	else
		camera->MoveCameraAround(msec);
	
	viewMatrix = camera->BuildViewMatrix();
	root->Update(msec);
	waterRotate += msec / 5000.0f;
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

void Renderer::DrawNode(SceneNode* n) {
	if (n->GetMesh()) {
		Matrix4 transform = n->GetWorldTransform() * Matrix4::Scale(n->GetModelScale());
		modelMatrix.ToIdentity();
		Matrix4 tempMatrix = textureMatrix * modelMatrix;
		glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(),"modelMatrix"), 1, false, (float*)&transform);
		glUniform1i(glGetUniformLocation(currentShader->GetProgram(),"useTexture"), (int)n->GetMesh()->GetTexture());
		glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "textureMatrix"), 1, false, *&tempMatrix.values);

		n->Draw(*this);
	}

	for(vector<SceneNode*>::const_iterator
		i = n->GetChildIteratorStart();
		i != n->GetChildIteratorEnd(); ++i) {
		DrawNode(*i);
	}

}

void Renderer::CreateForest(int amountTrees, Vector3 treeTransform, float scale, float degrees)
{
	SceneNode* forest = new SceneNode();
	forest->name = "forest";
	for (int i = 0; i < amountTrees; ++i) {
		Tree* newlyCreatedTree = new Tree();
		Matrix4 translationMatr = Matrix4::Translation(treeTransform + Vector3(i * 3.0f *scale, 0.0f, (i % 2) * 3.0f * scale));
		Matrix4 scalingMatr = Matrix4::Scale(Vector3(scale, scale, scale));
		newlyCreatedTree->SetTransform(newlyCreatedTree->GetTransform() * translationMatr * scalingMatr);
		forest->AddChild(newlyCreatedTree);
	}

	Matrix4 rotationMatr = Matrix4::Rotation(degrees, Vector3(0, 1, 0));
	//forest->SetBoundingRadius(2000.0f);
	forest->SetTransform(forest->GetTransform() * rotationMatr);
	root->AddChild(forest);
}



//----------------------------

void Renderer::DrawShadowScene() {
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);

	glClear(GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, 2048, 2048);

	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	SetCurrentShader(shadowShader);

	viewMatrix = Matrix4::BuildViewMatrix(light->GetPosition(), Vector3(0, 0, 0));
	textureMatrix = biasMatrix * (projMatrix * viewMatrix);

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