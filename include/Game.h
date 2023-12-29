#pragma once
#include "DirectXWindow.h"
#include "Graphics.h"
#include "ImGuiManager.h"
#include "Camera.h"
#include "Timer.h"
#include "PointLight.h"
#include "Spotlight.h"
#include "Interfaces.h"
#include "Model.h"
#include "XSResourse.h"

class Game
{
public:
	Game();
	~Game();
	int Start(int nCmdShow);
private:
	void UpdateFrame();
	void ShowControlWindow();
	void ShowItemsSubMenu();
	bool LoadConfigurationFile(const char* path);
	void UpdateConfigurationFile(const char* path);

	ImGuiManager ImMan = ImGuiManager();

	Camera cam;
	Timer timer;

	std::unique_ptr<DirectXWindow> window;
	std::unique_ptr<Graphics> gfx;

	std::vector<Drawable*> drawables;

	std::unique_ptr<Model> Tree = std::make_unique<Model>(*gfx, R"(.\Models\Tree\Tree.fbx)",
		aiProcess_CalcTangentSpace |
		aiProcess_GenNormals |
		aiProcess_Triangulate |
		aiProcess_ConvertToLeftHanded
	);
	std::unique_ptr<Model> Tree2  = std::make_unique<Model>(*gfx, R"(.\Models\Tree\Tree.fbx)",
		aiProcess_CalcTangentSpace |
		aiProcess_GenNormals |
		aiProcess_Triangulate |
		aiProcess_ConvertToLeftHanded
	);
	std::unique_ptr<Model> Lamp = std::make_unique<Model>(*gfx, R"(.\Models\bulb\bulb.obj)", 
		aiProcess_Triangulate
	);
	std::unique_ptr<Model> Sponza = std::make_unique<Model>(*gfx, R"(.\Models\Sponza\sponza.obj)", 
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate | 
		aiProcess_ConvertToLeftHanded
	);

	std::vector<std::unique_ptr<Light>> lightSources;
};