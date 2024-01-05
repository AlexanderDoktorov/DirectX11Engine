#include "Game.h"
#include <fstream>
#include <regex>
#include "DirectXTex.h"
#include "MaterialSystem.h"

#define NEAR_Z 2
#define FAR_Z 5000

Game::Game()
	: 
	window(std::make_unique<DirectXWindow>(L"Game", WS_OVERLAPPEDWINDOW)),
	gfx(std::make_unique<Graphics>(window->GetWnd()))
{
	gfx->SetRendererType(RENDERER_TYPE_FORWARD);
	// Lights
	gfx->AddLightSource(std::make_unique<PointLight>(*gfx));
	gfx->AddLightSource(std::make_unique<PointLight>(*gfx));
	//gfx->AddLightSource(std::make_unique<Spotlight>(*gfx));

	LoadModels();

#pragma region TEST
#pragma endregion TEST

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
	gfx->RenderToImGui(true);

	while (open)
	{
		// Messages
		constexpr float fov = dx::XM_PIDIV2;
		gfx->SetProjection(dx::XMMatrixPerspectiveFovLH(fov, 1, NEAR_Z, FAR_Z));
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

	const auto drawModels = [this]() {
		std::ranges::for_each(m_Models, [this](auto& model) {
			model->Draw(*gfx);
		});
	};

	// Fill G-buffer
	gfx->BeginFrame(window.get(), drawModels);
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
		if (std::optional<int> zDelta = window->ReadZDelta())
			cam.Accelerate(static_cast<float>(zDelta.value()) / 50.f);
	}
#ifndef _NOIMGUI
	ShowItemsSubMenu();
	cam.ShowControlWindow();
	for (size_t i = 0; i < m_Models.size(); i++) {
		m_Models[i]->ShowControlWindow(*gfx, m_Models[i]->GetName().append("##").append(std::to_string(i)));
	}
	gfx->ShowControlWindow("Graphics settings");
#endif
	gfx->EndFrame();
}

void Game::LoadModels()
{
	m_Models.push_back(std::make_unique<Model>(*gfx, R"(.\Models\Tree\Tree.fbx)",
		aiProcess_CalcTangentSpace |
		aiProcess_GenNormals |
		aiProcess_Triangulate |
		aiProcess_ConvertToLeftHanded
	));

	m_Models.push_back(std::make_unique<Model>(*gfx, R"(.\Models\Tree\Tree.fbx)",
		aiProcess_CalcTangentSpace |
		aiProcess_GenNormals |
		aiProcess_Triangulate |
		aiProcess_ConvertToLeftHanded
	));

	m_Models.push_back(std::make_unique<Model>(*gfx, R"(.\Models\bulb\bulb.obj)",
		aiProcess_Triangulate
	));

	m_Models.push_back(std::make_unique<Model>(*gfx, R"(.\Models\Sponza\sponza.obj)",
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_ConvertToLeftHanded
	));
	m_Models.back()->SetRootTransform(DirectX::XMMatrixScaling(1 / 20.f, 1 / 20.f, 1 / 20.f));

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
			for (unsigned int i = 0; i < drawables.size(); i++)
			{
				const bool is_selected = (current_item_selected == i);

				std::string str;
				if(auto labeled = dynamic_cast<IToString*>(drawables[i]))
					str.append(labeled->ToString());
				str.append(" " + std::to_string(i));

				if (ImGui::Selectable(str.c_str(), is_selected))
					current_item_selected = i;
			}
			ImGui::EndListBox();
		}

		if (drawables.empty()) {
			ImGui::End();
			return;
		}
		if(auto movable = dynamic_cast<IMovable*>(drawables[current_item_selected]))
		{
			dx::XMFLOAT3 pos = movable->GetPosition();
			if (ImGui::SliderFloat3("Item position", &pos.x, -100.f, 100.f))
				movable->SetPosition(pos.x, pos.y, pos.z);
		}
		if(auto scalable = dynamic_cast<IScalable*>(drawables[current_item_selected]))
		{
			dx::XMFLOAT3 scale = scalable->GetScale();
			if (ImGui::SliderFloat3("Item scale", &scale.x, 0.f, 100.f))
				scalable->SetScale(scale);
		}
		if(auto light = dynamic_cast<Light*>(drawables[current_item_selected]))
		{
			light->ShowLightGUI();
		}
	}
	ImGui::End();
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
	std::ofstream fout(path);

	fout << "[CAMERA]" << std::endl;
	fout << "POS=" << std::to_string(cam.GetPos().x) << ',' << std::to_string(cam.GetPos().y) << ',' << std::to_string(cam.GetPos().z) << std::endl;
	fout << "Pitch=" << std::to_string(cam.GetPitch()) << std::endl;
	fout << "Yaw=" << std::to_string(cam.GetYaw()) << std::endl;

	fout.close();
}


