#include "Application.h"

#ifndef _CRT_SECRUE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif // !1


INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpCmdLine, int nCmdShow)
{
	//Create new application
	Application* app = new Application();

	//If application is initialized run app
	if (app->initialize(L"Advanced Tech Project"))
	{
		app->run();
	}

	delete app;

	return 0;
}