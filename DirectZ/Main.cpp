#include "Common.h"

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPreviousInstance, LPSTR lpCmdLine,int nShowCmd)

{
	int AppWindow = Window::GetInstance(hInstance,nShowCmd,0,0,640,480,false)->MessageLoop();

	GraphicsDevice::Destroy();
	InputManager::Destroy();

	return AppWindow;
}