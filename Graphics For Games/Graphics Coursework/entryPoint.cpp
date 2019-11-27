/***********************************************
Course: CSC8502
Project: Coursework part A + part B
Author: Sofia Papadopoulou - 190338075
Acad. Year:	2019-2020
Git Repo: https://github.com/sopa92/Coursework_CSC8502.git
References: Textures used are taken from the following websites
https://www.sketchuptextureclub.com/, http://www.custommapmakers.org/, https://www.textures.com/, https://cdn.pixabay.com/
***********************************************/

#include "../../nclgl/window.h"
#include "Renderer.h"

#pragma comment(lib, "nclgl.lib")

int main() {
	Window w("CSC8502-Coursework-b9033807", 1920, 1080, true); // for FULLSCREEN
	//Window w("CSC8502-Coursework-b9033807", 1000, 600, false);
	if (!w.HasInitialised()) {
		return -1;
	}

	Renderer renderer(w);
	if (!renderer.HasInitialised()) {
		return -1;
	}

	w.LockMouseToWindow(true);
	w.ShowOSPointer(false);
	
	while (w.UpdateWindow() && !Window::GetKeyboard()->KeyDown(KEYBOARD_ESCAPE)) {
		renderer.UpdateScene(w.GetTimer()->GetTimedMS());		
		renderer.RenderScene();
	}

	return 0;
}

