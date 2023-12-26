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
#include "SolidBall.h"
#include "Sheet.h"
#include "SolidTexturedBall.h"

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

	std::vector<Drawable*> drawables;
	std::unique_ptr<Sheet> sheet;
	std::unique_ptr<Sheet> nmSheet;

	Model Tree;
	Model Tree2;
	Model Lamp;
	Model Sponza;

	std::vector<std::unique_ptr<SolidTexturedBall>> balls;
	std::unique_ptr<SolidBall> solidBall;

	std::vector<std::unique_ptr<Light>> lightSources;
};