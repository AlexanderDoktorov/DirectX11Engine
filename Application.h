#pragma once
#include "Window.h"
#include "Graphics.h"
#include "Box.h"
#include "ImGuiManager.h"

class Application
{
public:

	Application();

	int Start(int nCmdShow);

	~Application();

private:

	void UpdateFrame();

	ImGuiManager ImMan = ImGuiManager();
	std::unique_ptr<Window> window;
	std::unique_ptr<Graphics> gfx;
	Box<1.f>* box;
	Parallelogram<2.f,1.f,4.f>* large_box;
};