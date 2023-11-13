#pragma once
#include "DirectXWindow.h"
#include "Graphics.h"
#include "ImGuiManager.h"
#include "Camera.h"
#include "Items.h"
#include "Timer.h"
#include "LightSource.h"
#include "Interfaces.h"

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

	std::unique_ptr<DirectXWindow> window;
	std::unique_ptr<Graphics> gfx;

	std::unique_ptr<Bar> bar;
	std::vector<IObject*> objects;
	std::unique_ptr<Sheet> sheet;
	std::unique_ptr<Box> box;
	std::unique_ptr<Character> character;

	std::vector<std::unique_ptr<SolidLightenedBall>> balls;

	std::unique_ptr<LightSource> light;
	std::unique_ptr<LightSource> light2;
	std::vector<std::unique_ptr<LightSource>> lights;
};