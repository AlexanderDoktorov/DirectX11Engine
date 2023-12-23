// Programm that visualizes sorting
#include "Game.h"
#include <iostream>
#include "baseException.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	try
	{
		Game game = Game();
		return game.Start(nCmdShow);
	}
	catch (baseException& baseEx)
	{
		std::cerr << baseEx.what() << std::endl;
		MessageBoxA(NULL, baseEx.what().c_str(), "Exception info", MB_OK);
		return -1;
	}
	catch (std::exception& stdex)
	{
		std::cerr << stdex.what() << std::endl;
		MessageBoxA(NULL, stdex.what() , "Std exception info", MB_OK);
		return -1;
	}
	catch (...)
	{
		MessageBoxA(NULL, "Unknown exception", "Exception info", MB_OK);
		std::cerr << "Unknown exception" << std::endl;
		return -1;
	}
}
