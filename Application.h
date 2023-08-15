#pragma once
#include <DirectXColors.h>

#include "Window.h"
#include "Graphics.h"
#include "Histogramm.h"

#include "imgui.h"

class Application
{
public:

	Application();

	int Go(int nCmdShow);

private:

	bool IsSorting = false;
	bool FrameUpdated = false;

	void UpdateFrame();
	void ShakerSort(Histogramm& hist);
	void BubbleSort(Histogramm& hist);

	std::unique_ptr<Window> window;
	std::unique_ptr<Graphics> gfx;

	std::unique_ptr<Histogramm> hist;
	std::unique_ptr<Histogramm> initial_hist;
	std::unique_ptr<Column> clmn;
};