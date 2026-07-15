#include "AppWindow.h"
#include <timeapi.h>
#pragma comment(lib, "winmm.lib")

int main()
{
	timeBeginPeriod(1); // raise OS timer resolution so Sleep(1) ~= 1ms

	AppWindow app;
	if (app.init())
	{
		while (app.isRun())
		{
			app.broadcast();
		}
	}

	timeEndPeriod(1);
	return 0;
}