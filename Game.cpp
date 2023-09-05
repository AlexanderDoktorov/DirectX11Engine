#include "Game.h"
#include "Sorts.h"
#include "StringHelper.h"

#include <fstream>

Game::Game()
{
	// Graphics and window
	window			= std::make_unique<DirectXWindow>(L"Game", WS_OVERLAPPEDWINDOW);
	gfx				= std::make_unique<Graphics>(window->GetWnd());

	// Geometries
	box				= std::make_unique<Box>(*gfx);
	bar				= std::make_unique<Bar>(*gfx, 1.f,10.f,3.f);
	sheet			= std::make_unique<Sheet>(*gfx, dx::XMFLOAT4(0.5, 0.5, 0.5, 1.f));
	balls.push_back(std::make_unique<SolidLightenedBall>(*gfx, dx::XMFLOAT4(1.f, 0.f, 1.f, 1.f)));
	balls.push_back(std::make_unique<SolidLightenedBall>(*gfx, dx::XMFLOAT4(.5f, 0.1f, 1.f, 1.f)));

	// Lights
	lights.push_back(std::make_unique<LightSource>(*gfx));
	lights.push_back(std::make_unique<LightSource>(*gfx));

	box->SetPosition(0.f, 4.f, 5.f);
	box->Scale(0.5f);
	bar->SetPosition(4.f, 4.f, 5.f);
	bar->Scale(1.f, 2.f, 3.f);	

	balls[0]->SetWorldPosition(dx::XMFLOAT3(1.f, 5.f, 1.f));
	balls[1]->SetWorldPosition(dx::XMFLOAT3(1.f, 5.f, 10.f));

	for (auto& ball : balls)
		placable_items.push_back(ball.get());

	for (auto& light : lights)
		placable_items.push_back(light.get());

	LoadConfigurationFile("./game.config");
}

Game::~Game()
{
	UpdateConfigurationFile("./game.config");
}

int Game::Start(int nCmdShow)
{
	window->Show(nCmdShow);
	bool open = true;
	gfx->RenderToImGui(false);

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
	//gfx->BeginFrame(window.get(), color);

	// Fill G-buffer
	gfx->BeginGeometryPass(window.get(), color);	

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
		balls[0]->MakeSkeleton();

	for (auto& light_source : lights)
	{
		light_source->Draw(*gfx);
	}

	box->Draw(*gfx);
	bar->Draw(*gfx);
	for (auto& ball : balls)
	{
		ball->Draw(*gfx);
	}
	sheet->Draw(*gfx);

	// Apply light
	gfx->BeginLightningPass();

	for (auto& light_source : lights)
	{
		light_source->Bind(*gfx);
		gfx->BindLightBuffer();
		gfx->Draw(3U);
	}

	// Combine textures
	gfx->PerformCombinePass();

	//ShowControlWindow();
	//cam.ShowControlWindow();

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

bool Game::LoadConfigurationFile(const char* path)
{
	std::ifstream in_file_stream(path);

	if (!in_file_stream.is_open())
	{
		OutputDebugString(L"Unable to open configuration file");
		return false;
	}

	for (std::string line; std::getline(in_file_stream, line); )
	{
		if (line == "[CAMERA]")
		{
			std::getline(in_file_stream, line);

			std::stringstream  ss(line.substr(line.find("=") + 1));
			std::string buff;

			// Position
			std::getline(ss, buff, ',');
			cam.SetPosX(std::stof(buff));

			std::getline(ss, buff, ',');
			cam.SetPosY(std::stof(buff));

			std::getline(ss, buff, ',');
			cam.SetPosZ(std::stof(buff));

			// Rotation
			std::getline(in_file_stream, line);
			cam.SetPitch(std::stof(line.substr(line.find("=") + 1)));
			std::getline(in_file_stream, line);
			cam.SetYaw(std::stof(line.substr(line.find("=") + 1)));
		}
	}

	return true;
}

void Game::UpdateConfigurationFile(const char* path)
{
	std::ofstream fout("game.config");

	fout << "[CAMERA]" << std::endl;
	fout << "POS=" << std::to_string(cam.GetPos().x) << ',' << std::to_string(cam.GetPos().y) << ',' << std::to_string(cam.GetPos().z) << std::endl;
	fout << "Pitch=" << std::to_string(cam.GetPitch()) << std::endl;
	fout << "Yaw=" << std::to_string(cam.GetYaw()) << std::endl;

	fout.close();
}


