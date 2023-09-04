#pragma once
#include "BaseWindow.h"
#include "IResisingWindow.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "Camera.h"
#include "DOK_assert.h"
#include "RawInputDevices.h"
#include <memory>
#include <queue>
#include <optional>

#include "ImGui\backend\imgui_impl_win32.h"

class DirectXWindow : public BaseWindow<DirectXWindow>, public IResisingWindow
{
public:
    DirectXWindow(const wchar_t* Tag, DWORD dw_style)
    {
        Create(Tag, dw_style);
        devices.AddDevice(hWnd, mouse);
        devices.Register();
        ImGui_ImplWin32_Init(hWnd);
    }

    ~DirectXWindow()
    {
        ImGui_ImplWin32_Shutdown();
    }

    LPCWSTR ClassName() const noexcept override { return L"DirectX11 Example"; }
    LRESULT CALLBACK HandeMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

    const Mouse&            GetMouse() const { return mouse; }
    const Keyboard&         GetKeyboard() const { return keyboard; }
    const bool              MouseCaptured() const { return captured; }
    std::optional<RawData>  ReadRawDelta();

private:

    void CaptureCursor();
    void ReleaseCursor();
    void UpdateRawInputData(const LPARAM& lParam);

    RawInputDevices     devices;
    Mouse               mouse;
    Keyboard            keyboard;
    std::queue<RawData> rawDeltaQueue;
    bool                captured    = false;
};