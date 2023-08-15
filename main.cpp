// Programm that visualizes sorting
#include "Application.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	try
	{
		Application app = Application();
		return app.Go(nCmdShow);
	}
	catch (hrException& hr_ex)
	{
		OutputDebugStringW(hr_ex.what());
		return -1;
	}
}

