#pragma once
#include <Windows.h>
#include <hidusage.h>
#include <vector>
#include "DOK_assert.h"

class RawInputDevices
{
public:
	RawInputDevices() {}

	RawInputDevices(HWND hwnd, USAGE UsagePage, USAGE UsageID)
	{
		AddDevice(hwnd, UsagePage, UsageID);
	}

	void AddDevice(HWND hwnd, USAGE UsagePage, USAGE UsageID)
	{
		RAWINPUTDEVICE device{};
		device.hwndTarget = hwnd;
		device.usUsagePage = UsagePage;// the class of the device ( e.g. mouse)
		device.usUsage = UsageID; // the device within the class ( e.g. first mouse)
		device.dwFlags = 0; // default
		devices.push_back(device);
	}

	void Register()
	{
		BOOL result = RegisterRawInputDevices(devices.data(), (UINT)devices.size(), sizeof(RAWINPUTDEVICELIST) );
		DOK_assert(result == TRUE, L"Failed to register devices");
	}

private:
	std::vector<RAWINPUTDEVICE> devices;
};