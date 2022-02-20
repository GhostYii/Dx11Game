#include "AppWindow.h"

int main()
{
	try
	{
		InputSystem::CreateInstance();
		GraphicsEngine::CreateInstance();
	}
	catch (...)
	{
		return -1;
	}

	try
	{
		AppWindow app;
		while (app.IsRun());
	}
	catch (...)
	{
		GraphicsEngine::ReleaseInstance();
		InputSystem::ReleaseInstance();
		return -1;
	}

	GraphicsEngine::ReleaseInstance();
	InputSystem::ReleaseInstance();

	return 0;
}