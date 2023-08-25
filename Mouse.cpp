#include "Mouse.h"

void Mouse::ShowMenu(bool* p_open) const
{
	if (ImGui::Begin("Mouse data", p_open))
	{
		ImGui::Text("RawData X(%d), Y(%d)", rawData.rawX, rawData.rawY);
	}
	ImGui::End();
}

bool Mouse::IsLMBPressed() const
{
	return IsKeyDown(VK_LBUTTON);
}

bool Mouse::IsRMBPressed() const
{
	return IsKeyDown(VK_RBUTTON);
}

void Mouse::OnDeltaRaw(const LONG& lLastX, const LONG& lLastY)
{
	rawData.rawX += lLastX;
	rawData.rawY += lLastY;
}

RawData Mouse::GetRawData() const
{
	return rawData;
}
