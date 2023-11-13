#pragma once
#include <Windows.h>
#include "Device.h"
#include "imgui.h"

struct RawData
{
	RawData() = default;
	RawData(const LONG& rawX, const LONG& rawY)
		: rawX(rawX), rawY(rawY)
	{
	}

	bool operator!=(const RawData& other) { return rawX != other.rawX && rawY != other.rawY; }
	RawData operator-(const RawData& other) { return RawData(rawX - other.rawX, rawY - other.rawY); }

	LONG rawX = 0;
	LONG rawY = 0;
};

struct Position
{
	Position() = default;

	int X = 0;
	int Y = 0;
};

class Mouse : public Device
{
public:
	Mouse() : Device(HID_USAGE_PAGE_GENERIC, HID_USAGE_GENERIC_MOUSE) {}

	void	ShowMenu(bool* p_open = (bool*)0) const;

	bool	IsLMBPressed() const;
	bool	IsRMBPressed() const;

	void	OnDeltaRaw(const LONG& lLastX, const LONG& lLastY);
	RawData GetRawData() const;

private:
	Position position;
	RawData rawData;
};