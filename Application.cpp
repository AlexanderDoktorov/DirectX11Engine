#include "Application.h"
#include "Sorts.h"

#include <chrono>
#include <thread>

Application::Application()
{
	window = std::make_unique<Window>(L"Sorting visualizer", WS_OVERLAPPEDWINDOW);
	window->AddCamera(&cam);
	gfx = std::make_unique<Graphics>(window->GetWnd());

	Box::Make(*gfx);
	box = std::make_unique<Box>(*gfx);
	parallelogramm = std::make_unique<Parallelogram>(*gfx, 1.f,10.f,3.f);

	box->SetPosition(0.f, 0.f, 10.f);
	box->Scale(0.5f);
	parallelogramm->SetPosition(2.f, 0.f, 10.f);
	parallelogramm->Scale(1.f, 2.f, 3.f);	
}

int Application::Start(int nCmdShow)
{
	window->Show(nCmdShow);
	bool open = true;

	while (open)
	{
		// Messages
		gfx->SetProjection(cam.GetCameraMatrix() * dx::XMMatrixPerspectiveLH(1.f, (float)window->GetWidth() / window->GetHeight(), 1.f, 40.f));
		if (const auto r = window->ProcessMessage())
		{
			open = false;
			return r.value();
		}
		UpdateFrame();
	}

	return 0;
}

void Application::UpdateFrame()
{
	// Graphics
	const float color[4] = { 0.2f, 0.2f, 0.2f , 0.1f };
	gfx->BeginFrame(window.get(), color);

	cam.ShowControlWindow();
	window->GetMouse().ShowMenu();

	box->Draw(*gfx);
	parallelogramm->Draw(*gfx);

	gfx->EndFrame();
}


