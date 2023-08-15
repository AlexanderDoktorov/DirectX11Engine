#pragma once
#include "ResizingBaseWindow.h"
#include <memory>

#include "ImGui\backend\imgui_impl_win32.h"

class Window : public ResizingBaseWindow<Window>
{
public:
    Window(const wchar_t* Tag, DWORD dw_style)
    {
        Create(Tag, dw_style);
        ImGui_ImplWin32_Init(hWnd);
    }

    ~Window()
    {
        ImGui_ImplWin32_Shutdown();
    }

    LPCWSTR ClassName() const noexcept override { return L"DirectX11 Example"; }
    LRESULT CALLBACK HandeMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

private:
    class Mouse
    {
    public:
        Mouse() : pos_x(0U), pos_y(0U) {}
        UINT pos_x;
        UINT pos_y;
    } Mouse;
public:
    UINT  MousePosX() { return Mouse.pos_x; }
    UINT  MousePosY() { return Mouse.pos_y; }
    float NormalMousePosX() { return  2.f * (Mouse.pos_x / (float)GetHeight()) - 1.f; }
    float NormalMousePosY() { return -2.f * (Mouse.pos_y / (float)GetWidth()) + 1.f; }
};