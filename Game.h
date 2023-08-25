#pragma once
#include "Window.h"
#include "Graphics.h"
#include "ImGuiManager.h"
#include "Camera.h"
#include "Items.h"
#include "Timer.h"
#include "LightSource.h"


class Game
{
public:

	Game();

	int Start(int nCmdShow);

private:

	void UpdateFrame();

	ImGuiManager ImMan = ImGuiManager();

	Camera cam;
	Timer timer;

	std::unique_ptr<Window> window;
	std::unique_ptr<Graphics> gfx;
	std::unique_ptr<Bar> bar;
	std::unique_ptr<SolidLightenedBall> ball;
	std::unique_ptr<Sheet> sheet;
	std::unique_ptr<SolidLightenedBall> ball_1;
	std::unique_ptr<Box> box;

	std::unique_ptr<LightSource> light;
	std::unique_ptr<LightSource> light2;
};