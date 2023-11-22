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
		OutputDebugStringW(L" *************** EXCEPTION OCCURED *************** \n");
		OutputDebugStringW(hr_ex.what());
		OutputDebugStringW(L"\n");
		OutputDebugStringW(L" *************** EXCEPTION OCCURED *************** \n");
		return -1;
	}
}
