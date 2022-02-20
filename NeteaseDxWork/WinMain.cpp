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

	AppWindow app;

	if (app.Init())
	{
		while (app.IsRun())
		{
			app.Broadcast();
		}
	}

	GraphicsEngine::ReleaseInstance();
	InputSystem::ReleaseInstance();

	return 0;
}