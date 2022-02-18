#include "AppWindow.h"

int main()
{
	AppWindow app;

	if (app.Init())
	{
		while (app.IsRun())
		{
			app.Broadcast();
		}
	}

	return 0;
}