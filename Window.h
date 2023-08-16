#pragma once
#include "ResizingBaseWindow.h"
#include "Camera.h"
#include "Mouse.h"
#include "DOK_assert.h"
#include "RawInputDevices.h"
#include <memory>

#include "ImGui\backend\imgui_impl_win32.h"

class Window : public ResizingBaseWindow<Window>
{
public:
    Window(const wchar_t* Tag, DWORD dw_style)
    {
        Create(Tag, dw_style);
        devices.AddDevice(hWnd, HID_USAGE_PAGE_GENERIC, HID_USAGE_GENERIC_MOUSE);
        devices.Register();
        ImGui_ImplWin32_Init(hWnd);
    }

    ~Window()
    {
        ImGui_ImplWin32_Shutdown();
    }

    LPCWSTR ClassName() const noexcept override { return L"DirectX11 Example"; }
    LRESULT CALLBACK HandeMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;
    Mouse& GetMouse() { return mouse; }

private:
    bool captured = false;
    void CaptureCursor();
    void ReleaseCursor();
private:
    Camera* camera = nullptr;
public:
    void AddCamera(Camera* _camera) { DOK_assert(_camera != nullptr, L"Empty camera!"); camera = _camera; }
private:
    RawInputDevices devices;
    Mouse mouse;
};