#pragma once
#include "Window.h"
#include "Graphics.h"
#include "ImGuiManager.h"

#include "ItemFactory.h"
#include "Camera.h"

class Application
{
public:

	Application();

	int Start(int nCmdShow);

private:

	void UpdateFrame();

	ImGuiManager ImMan = ImGuiManager();
	Camera cam;

	std::unique_ptr<Window> window;
	std::unique_ptr<Graphics> gfx;
	std::unique_ptr<Parallelogram> parallelogramm;
	std::unique_ptr<Box> box;
};