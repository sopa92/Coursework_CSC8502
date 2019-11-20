#include "../../nclgl/window.h"
#include "Renderer.h"

#pragma comment(lib, "nclgl.lib")

int main() {
	Window w("CSC8502-Coursework-b9033807", 1000, 700, false);
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