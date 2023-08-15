#include "Application.h"
#include "Sorts.h"

#include <chrono>
#include <thread>

constexpr auto columns_count = (unsigned int)2U;

Application::Application()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
	ImGui::StyleColorsDark();

	window = std::make_unique<Window>(L"Sorting visualizer", WS_OVERLAPPEDWINDOW);;
	gfx = std::make_unique<Graphics>(window->GetWnd());

	rectangle rect =
	{
		dx::XMFLOAT3(-1.f, -1.f, 0.f),
		dx::XMFLOAT3(1.f, -1.f, 0.f),
		dx::XMFLOAT3(1.f, 1.f, 0.f),
		dx::XMFLOAT3(-1.f, 1.f, 0.f)
	};

	hist = std::make_unique<Histogramm>(*gfx, rect);

	for (size_t i = 0; i < 100; i++)
	{
		hist->PushColumn(*gfx, rand() % 100);
	}
}

int Application::Go(int nCmdShow)
{
	window->Show(nCmdShow);

	gfx->SetProjection(dx::XMMatrixIdentity());
	bool open = true;

	while (open)
	{
		// Messages
		if (const auto r = window->ProcessMessage())
		{
			open = false;
			return r.value();
		}
		ShakerSort(*hist);
		UpdateFrame();
	}

	return 0;
}


void Application::ShakerSort(Histogramm& hist) {
	if (hist.columns.empty()) {
		return;
	}
	int left = 0;
	int right = hist.columns.size() - 1;
	if (left <= right) {
		for (int i = right; i > left; --i) {
			if (*hist.columns[i - 1] > *hist.columns[i]) {
				hist.columns[i - 1].swap(hist.columns[i]);
				hist.UpdateHistogramm();
				UpdateFrame();
			}
		}
		++left;
		for (int i = left; i < right; ++i) {
			if (*hist.columns[i] > *hist.columns[i + 1]) {
				hist.columns[i].swap(hist.columns[i + 1]);
				hist.UpdateHistogramm();
				UpdateFrame();
			}
		}
		--right;
	}
}

void Application::BubbleSort(Histogramm& hist) 
{
	static int i = 0;
	static int j = 0;

	if (j + 1 == hist.columns.size() - i)
	{
		j = 0;
		i++;
	}
	if (j + 1 > hist.columns.size() - 1 ) return;

	if (*hist.columns[j] > *hist.columns[j + 1]) {
		hist.columns[j].swap(hist.columns[j + 1]);
		hist.UpdateHistogramm();
	}

	j++;
}

void Application::UpdateFrame()
{
	// Graphics
	const float color[4] = { 0.2f, 0.2f, 0.2f , 0.1f };
	gfx->BeginFrame(window.get(), color);

	if (ImGui::Begin("Controls"))
	{
		auto& io = ImGui::GetIO();
		ImGui::Text("FPS %.1f", 1.0f / io.DeltaTime);

		if (ImGui::Button("Reset"))
		{
			hist->Clear();
			for (size_t i = 0; i < 100; i++)
			{
				hist->PushColumn(*gfx, rand() % 100);
			}
		}
		if (ImGui::Button("Switch state"))
			IsSorting ? IsSorting = false : IsSorting = true;

		const char* sorting_methods[] = { "bubble sort", "shaker sort", "choice sort", "quick_sort"};
		static int item_idx = 0;
		if (ImGui::BeginListBox("Sorting methods"))
		{
			for (int n = 0; n < IM_ARRAYSIZE(sorting_methods); n++)
			{
				const bool is_selected = (item_idx == n);
				if (ImGui::Selectable(sorting_methods[n], is_selected))
					item_idx = n;

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndListBox();
		}
	}
	ImGui::End();

	hist->Draw(*gfx);

	//std::this_thread::sleep_for(std::chrono::duration(std::chrono::seconds(1)));

	gfx->EndFrame();
}


