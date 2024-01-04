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

static std::mutex mutex;

class Game
{
public:
	Game();
	~Game();
	int Start(int nCmdShow);
private:
	void UpdateFrameForwardly();
	void UpdateFrameDefferdly();
	void LoadModels();
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
	std::vector<std::unique_ptr<Model>> m_Models;
	std::vector<std::unique_ptr<Light>> lightSources;
};