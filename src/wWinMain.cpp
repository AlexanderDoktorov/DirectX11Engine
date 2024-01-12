// Programm that visualizes sorting
#include "Game.h"
#include <iostream>
#include "baseException.h"

#ifdef _DEBUG
#define _LOGGING_EN
#endif _DEBUG

#include "Logger.h"
#include "Testing.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	// std::cerr and std::cout now writes into files of logger
	logger.RedirectErrors(&std::cerr);
	logger.RedirectLog(&std::cout);

	try
	{
		DoTest();

		HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED); CHECK_HR(hr);
		Game game = Game();
		auto val =  game.Start(nCmdShow);
		CoUninitialize();
		return 0;
	}
	catch (baseException& baseEx)
	{
		std::cerr << baseEx.what() << std::endl;
		MessageBoxA(nullptr, baseEx.what().c_str(), "Exception info", MB_OK);
	}
	catch (std::exception& stdex)
	{
		std::cerr << stdex.what() << std::endl;
		MessageBoxA(nullptr, stdex.what() , "Std exception info", MB_OK);
	}
	catch (...)
	{
		MessageBoxA(nullptr, "Unknown exception", "Exception info", MB_OK);
		std::cerr << "Unknown exception" << std::endl;
	}
	return -1;
}
