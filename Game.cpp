#include "Game.h"
#include "Sorts.h"

#include <chrono>
#include <thread>

Game::Game()
{
	window			= std::make_unique<Window>(L"Game", WS_OVERLAPPEDWINDOW);
	gfx				= std::make_unique<Graphics>(window->GetWnd());

	box				= std::make_unique<Box>(*gfx);
	light			= std::make_unique<LightSource>(*gfx);
	light2			= std::make_unique<LightSource>(*gfx);
	bar				= std::make_unique<Bar>(*gfx, 1.f,10.f,3.f);
	ball			= std::make_unique<SolidLightenedBall>(*gfx, dx::XMFLOAT4(1.f, 0.f, 1.f, 1.f));
	ball_1			= std::make_unique<SolidLightenedBall>(*gfx, dx::XMFLOAT4(.5f, 0.1f, 1.f, 1.f));
	sheet			= std::make_unique<Sheet>(*gfx, dx::XMFLOAT4(0.5, 0.5, 0.5, 1.f));

	box->SetPosition(0.f, 4.f, 5.f);
	box->Scale(0.5f);
	bar->SetPosition(4.f, 4.f, 5.f);
	bar->Scale(1.f, 2.f, 3.f);	

	ball_1->SetPosition(dx::XMFLOAT3(1.f, 5.f, 1.f));
	ball->SetPosition(dx::XMFLOAT3(1.f, 5.f, 10.f));
}

int Game::Start(int nCmdShow)
{
	window->Show(nCmdShow);
	bool open = true;

	while (open)
	{
		// Messages
		gfx->SetProjection(dx::XMMatrixPerspectiveLH(1.f, (float)window->GetWidth() / window->GetHeight(), 1.f, 500.f));
		gfx->SetCamera(cam);
		if (const auto r = window->ProcessMessage())
		{
			open = false;
			return r.value();
		}
		UpdateFrame();
	}

	return 0;
}

void Game::UpdateFrame()
{
	// Graphics
	auto dt = timer.Check();

	const float color[4] = { 0.2f, 0.2f, 0.2f , 0.1f };
	gfx->BeginFrame(window.get(), color);


	if (window->MouseCaptured())
	{
		while (const auto deltaraw = window->ReadRawDelta())
		{
			cam.Rotate((float)deltaraw->rawX, (float)deltaraw->rawY);
		}

		if (window->GetKeyboard().IsKeyDown(BUTTON_W)) // forward
			cam.Translate({ 0.0f,0.0f,dt });
		if (window->GetKeyboard().IsKeyDown(BUTTON_S))
			cam.Translate({ 0.0f,0.0f,-dt });
		if (window->GetKeyboard().IsKeyDown(BUTTON_A))
			cam.Translate({ -dt,0.0f,0.0f });
		if (window->GetKeyboard().IsKeyDown(BUTTON_D))
			cam.Translate({ dt,0.0f,0.0f });
		if (window->GetKeyboard().IsKeyDown(VK_SPACE))
			cam.Translate({ 0.0f,dt,0.0f });
		if (window->GetKeyboard().IsKeyDown(VK_SHIFT))
			cam.Translate({ 0.0f,-dt,0.0f });
	}

	if (window->GetKeyboard().IsKeyDown(Button::BUTTON_K))
	{
		ball->MakeSkeleton();
	}

	light->Bind(*gfx);
	light->Draw(*gfx);

	box->Draw(*gfx);
	bar->Draw(*gfx);
	ball->Draw(*gfx);
	ball_1->Draw(*gfx);
	sheet->Draw(*gfx);

	light->ShowControlWindow();
	ImGui::ShowDemoWindow();
	gfx->ShowRenderWindow();
	
	//cam.ShowControlWindow();
	//window->GetMouse().ShowMenu();
	//window->PutIntoImGuiWnd(*gfx);

	gfx->EndFrame();
}


