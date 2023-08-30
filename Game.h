#pragma once
#include "Window.h"
#include "Graphics.h"
#include "ImGuiManager.h"
#include "Camera.h"
#include "Items.h"
#include "Timer.h"
#include "LightSource.h"
#include "IPlacableItem.h"

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
	void CreateBall();
	bool LoadConfigurationFile(const char* path);
	void UpdateConfigurationFile(const char* path);

	ImGuiManager ImMan = ImGuiManager();

	Camera cam;
	Timer timer;

	std::unique_ptr<Window> window;
	std::unique_ptr<Graphics> gfx;

	std::unique_ptr<Bar> bar;
	std::vector<IPlacableItem*> placable_items;
	std::unique_ptr<Sheet> sheet;
	std::unique_ptr<Box> box;

	std::vector<std::unique_ptr<SolidLightenedBall>> balls;

	std::unique_ptr<LightSource> light;
	std::unique_ptr<LightSource> light2;
};