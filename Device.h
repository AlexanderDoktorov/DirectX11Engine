#pragma once
#include <hidusage.h>
#include <Windows.h>

class Device
{
public:
	Device() = default;

	Device(USAGE UsagePage, USAGE UsageID) 
		:	UsageID(UsageID),
			UsagePage(UsagePage)
	{}

	bool IsKeyDown(int vVirtKey) const
	{
		return GetKeyState(vVirtKey) & 0x8000;
	}

	virtual USAGE GetUsagePage() const noexcept { return UsagePage; }
	virtual USAGE GetUsageID()	 const noexcept { return UsageID; }

private:
	USAGE UsagePage; 
	USAGE UsageID;
};