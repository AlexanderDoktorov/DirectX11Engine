// Programm that visualizes sorting
#include "Game.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	try
	{
		Game game = Game();
		return game.Start(nCmdShow);
	}
	catch (hrException& hr_ex)
	{
		OutputDebugStringW(hr_ex.what());
		return -1;
	}
}
