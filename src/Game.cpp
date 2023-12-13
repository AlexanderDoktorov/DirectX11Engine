#include "Game.h"
#include "StringHelper.h"
#include <fstream>
#include <regex>

#include "BindableSystem.h"

Game::Game()
{
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED); CHECK_HR(hr);
	// Graphics and window
	window			= std::make_unique<DirectXWindow>(L"Game", WS_OVERLAPPEDWINDOW);
	gfx				= std::make_unique<Graphics>(window->GetWnd());

	// Geometries
	sheet			= std::make_unique<Sheet>(*gfx, dx::XMFLOAT4(0.5, 0.5, 0.5, 1.f));
	solidBall       = std::make_unique<SolidBall>(*gfx);

	balls.push_back(std::make_unique<SolidTexturedBall>(*gfx, 1.f));
	balls.push_back(std::make_unique<SolidTexturedBall>(*gfx, 1.f));

	// Lights
	lights.push_back(std::make_unique<LightSource>(*gfx));

	sheet->Scale(20.f, 20.f, 20.f);

	// Models
	Tree.Load(*gfx, R"(G:\Visual Studio Projects\ProjForTests\Models\Tree\Tree.fbx)",
		aiProcess_CalcTangentSpace |
		aiProcess_GenNormals |
		aiProcess_Triangulate |
		aiProcess_ConvertToLeftHanded
	);

	Tree2.Load(*gfx, R"(G:\Visual Studio Projects\ProjForTests\Models\Tree\Tree.fbx)",
		aiProcess_CalcTangentSpace |
		aiProcess_GenNormals |
		aiProcess_Triangulate |
		aiProcess_ConvertToLeftHanded
	);
	Tree2.Translate(10, 0, 0);

	Lamp.Load(*gfx, R"(G:\Visual Studio Projects\ProjForTests\Models\bulb\bulb.obj)", 
		aiProcess_Triangulate
	);

	balls[0]->SetPosition(1.f, 5.f, 1.f);
	balls[1]->SetPosition(1.f, 5.f, 10.f);

	for (auto& ball : balls)
		objects.push_back(ball.get());

	for (auto& light : lights)
		objects.push_back(light.get());

#pragma region TEST
#pragma endregion TEST

	LoadConfigurationFile("./game.config");
}

Game::~Game()
{
	UpdateConfigurationFile("./game.config");
	CoUninitialize();
}

int Game::Start(int nCmdShow)
{
	window->Show(nCmdShow);
	bool open = true;
	gfx->RenderToImGui(true);

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

	// Fill G-buffer
	gfx->BeginGeometryPass(window.get());
	{
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

		for (auto& light_source : lights)
		{
			light_source->Draw(*gfx);
		}

		for (auto& ball : balls)
		{
			ball->Draw(*gfx);
		}
		Tree.Draw(*gfx);
		Tree2.Draw(*gfx);
		Lamp.Draw(*gfx);
	}
	gfx->EndGeometryPass();

	// Apply light
	gfx->BeginLightningPass();
	{
		for (auto& light_source : lights)
		{
			light_source->Bind(*gfx);
			//light_source->Update(dt);
			gfx->Draw(3U);
		}
	}
	gfx->EndLightningPass();

	// Combine textures
	gfx->PerformCombinePass();

#ifndef _NOIMGUI
	ShowControlWindow();
	cam.ShowControlWindow();
	Tree.ShowControlWindow(*gfx, "Tree controls");
	Tree2.ShowControlWindow(*gfx, "Tree2 controls");
	Lamp.ShowControlWindow(*gfx, "Lamp controls");
	gfx->GetMaterialSystem().ShowMaterialsWindow();
#endif
	gfx->EndFrame();
}

void Game::ShowControlWindow()
{
	ImGui::Begin("Controls");
	{
		//light->ShowControlChildWindow();
		ImGui::Separator();
		ShowItemsSubMenu();
	}
	ImGui::End();
}

void Game::ShowItemsSubMenu()
{
	if(ImGui::Begin("Objects creation"))
	{
		static unsigned int current_item_selected = 0;
		if(ImGui::BeginListBox("Items", ImVec2(200, 100)))
		{
			for (unsigned int i = 0; i < objects.size(); i++)
			{
				const bool is_selected = (current_item_selected == i);

				std::string str;
				if(auto labeled = dynamic_cast<IToString*>(objects[i]))
					str.append(labeled->ToString());
				str.append(" " + std::to_string(i));

				if (ImGui::Selectable(str.c_str(), is_selected))
					current_item_selected = i;
			}
			ImGui::EndListBox();
		}
		

		ImGui::SameLine();
		if (ImGui::Button("Create ball"))
			CreateBall();

		if(auto movable = dynamic_cast<IMovable*>(objects[current_item_selected]))
		{
			dx::XMFLOAT3 pos = movable->GetPosition();
			if (ImGui::SliderFloat3("Item position", &pos.x, -100.f, 100.f))
				movable->SetPosition(pos.x, pos.y, pos.z);
		}
		if(auto scalable = dynamic_cast<IScalable*>(objects[current_item_selected]))
		{
			dx::XMFLOAT3 scale = scalable->GetScale();
			if (ImGui::SliderFloat3("Item scale", &scale.x, 0.f, 100.f))
				scalable->SetScale(scale);
		}
		if(auto colored = dynamic_cast<IColored*>(objects[current_item_selected]))
		{
			dx::XMFLOAT4 color = colored->GetColor();
			if (ImGui::ColorPicker4("Item color", &color.x))
				colored->SetColor(color);
		}
		if(auto lightSource = dynamic_cast<ILight*>(objects[current_item_selected]))
		{
			auto lightDesc = lightSource->GetDesc();
			if (ImGui::ColorPicker3("Diffuse color", &lightDesc.diffuseColor.x))
				lightSource->SetDiffuseColor(lightDesc.diffuseColor);
			if (ImGui::SliderFloat("Diffuse Intensity", &lightDesc.diffuseIntensity, 0.f, 100.f))
				lightSource->SetDiffuseIntensity(lightDesc.diffuseIntensity);
			if (ImGui::SliderFloat("Constant Attenuation", &lightDesc.Catt, 0.f, 100.f))
				lightSource->SetConstantAttenuation(lightDesc.Catt);
			if (ImGui::SliderFloat("Linear Attenuation", &lightDesc.Latt, 0.f, 100.f))
				lightSource->SetLinearAttenuation(lightDesc.Latt);
			if (ImGui::SliderFloat("Quad Attenuation", &lightDesc.Qatt, 0.f, 100.f))
				lightSource->SetQuadAttenuation(lightDesc.Qatt);
		}
	}
	ImGui::End();
}

void Game::CreateBall()
{
	balls.push_back(std::make_unique<SolidTexturedBall>(*gfx));
	objects.push_back(balls.back().get());
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


