#include "GameWindow.h"

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
		GameWindow game;
		while (game.IsRun());
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