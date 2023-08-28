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
	bar				= std::make_unique<Bar>(*gfx, 1.f,10.f,3.f);
	sheet			= std::make_unique<Sheet>(*gfx, dx::XMFLOAT4(0.5, 0.5, 0.5, 1.f));
	balls.push_back(std::make_unique<SolidLightenedBall>(*gfx, dx::XMFLOAT4(1.f, 0.f, 1.f, 1.f)));
	balls.push_back(std::make_unique<SolidLightenedBall>(*gfx, dx::XMFLOAT4(.5f, 0.1f, 1.f, 1.f)));

	box->SetPosition(0.f, 4.f, 5.f);
	box->Scale(0.5f);
	bar->SetPosition(4.f, 4.f, 5.f);
	bar->Scale(1.f, 2.f, 3.f);	

	balls[0]->SetWorldPosition(dx::XMFLOAT3(1.f, 5.f, 1.f));
	balls[1]->SetWorldPosition(dx::XMFLOAT3(1.f, 5.f, 10.f));

	placable_items.push_back(balls[0].get());
	placable_items.push_back(balls[1].get());
	placable_items.push_back(light.get());
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
		gfx->RenderToImGui(true);
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
		balls[0]->MakeSkeleton();
	}

	light->Bind(*gfx);
	light->Draw(*gfx);

	box->Draw(*gfx);
	bar->Draw(*gfx);
	for (auto& ball : balls)
	{
		ball->Draw(*gfx);
	}
	sheet->Draw(*gfx);

	ShowControlWindow();

	gfx->EndFrame();
}

void Game::ShowControlWindow()
{
	ImGui::Begin("Controls");
	{
		light->ShowControlChildWindow();
		ImGui::Separator();
		ShowItemsSubMenu();
	}
	ImGui::End();
}

void Game::ShowItemsSubMenu()
{
	ImGui::BeginChild("Objects creation", ImVec2(600, 200), true);
	{
		static unsigned int current_item_selected = 0;
		ImGui::BeginListBox("Items", ImVec2(200, 100));
		{
			for (unsigned int i = 0; i < placable_items.size(); i++)
			{
				const bool is_selected = (current_item_selected == i);

				std::string str;
				if(auto labeled = dynamic_cast<IToString*>(placable_items[i]))
					str.append(labeled->ToString());
				str.append(" " + std::to_string(i));

				if (ImGui::Selectable(str.c_str(), is_selected))
					current_item_selected = i;
			}
		}
		ImGui::EndListBox();

		ImGui::SameLine();
		if (ImGui::Button("Create ball"))
		{
			CreateBall();
		}

		static dx::XMFLOAT3 pos = { 0,0,0 };
		if (ImGui::SliderFloat3("Item position", &pos.x, -100.f, 100.f)) placable_items[current_item_selected]->SetWorldPosition(pos);
	}
	ImGui::EndChild();
}

void Game::CreateBall()
{
	balls.push_back(std::make_unique<SolidLightenedBall>(*gfx));
	placable_items.push_back(balls.back().get());
}


