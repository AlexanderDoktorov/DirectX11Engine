#pragma once
#include <Windows.h>
#include "Device.h"
#include <vector>
#include "DOK_assert.h"

class RawInputDevices
{
public:
	RawInputDevices() {}

	RawInputDevices(HWND hwnd, const Device& device)
	{
		AddDevice(hwnd, device);
	}

	void AddDevice(HWND hwnd, const Device& dev)
	{
		RAWINPUTDEVICE device{};
		device.hwndTarget = hwnd;
		device.usUsagePage = dev.GetUsagePage();// the class of the device ( e.g. mouse)
		device.usUsage = dev.GetUsageID(); // the device within the class ( e.g. first mouse)
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