#pragma once
#include <Windows.h>
#include "imgui.h"

struct RawData
{
	RawData() = default;

	LONG rawX = 0;
	LONG rawY = 0;
};

struct Position
{
	Position() = default;

	int X = 0;
	int Y = 0;
};

class Mouse
{
public:
	Mouse() {}

	//float   NormalMousePosX() { return  2.f * (Mouse.pos_x / (float)GetHeight()) - 1.f; }
	//float   NormalMousePosY() { return -2.f * (Mouse.pos_y / (float)GetWidth()) + 1.f; }

	void ShowMenu(bool* p_open = (bool*)0)
	{
		if(ImGui::Begin("Mouse data", p_open))
		{
			ImGui::Text("RawData X(%d), Y(%d)", rawData.rawX, rawData.rawY);
		}
		ImGui::End();
	}

	void OnDeltaRaw(const LONG& lLastX, const LONG& lLastY);
	RawData GetRawData() const { return rawData; }

private:
	Position position;
	RawData rawData;
};