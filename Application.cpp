#include "Application.h"
#include "Sorts.h"

#include <chrono>
#include <thread>

#include "IItemFactory.h"

Application::Application()
{
	window = std::make_unique<Window>(L"Sorting visualizer", WS_OVERLAPPEDWINDOW);;
	gfx = std::make_unique<Graphics>(window->GetWnd());


	IItemFactory::CreateBox(*gfx, &box);
	IItemFactory::CreateParallelogramm(*gfx, &large_box);
	box->SetPosition(0.f, 0.f, 10.f);
	large_box->SetPosition(2.f, 0.f, 10.f);
}

Application::~Application()
{
	if (box) delete box;
	if (large_box) delete large_box;
}

int Application::Start(int nCmdShow)
{
	window->Show(nCmdShow);

	gfx->SetProjection(dx::XMMatrixPerspectiveLH(window->GetWidth() / 1920.f, window->GetHeight() / 1080.f, 1.f, 40.f));
	bool open = true;

	while (open)
	{
		// Messages
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

	box->Draw(*gfx);
	large_box->Draw(*gfx);

	gfx->EndFrame();
}


