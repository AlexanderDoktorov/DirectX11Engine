#include "DirectXWindow.h"
#include "imgui_impl_win32.h"
#include <windowsx.h>
#include <strsafe.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT DirectXWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
        return true;

    switch (uMsg)
    {
    case WM_KEYDOWN:
        if(wParam == Button::BUTTON_C)
        {
            if (!captured)
                CaptureCursor();
            else
                ReleaseCursor();
            break;
        }
        break;
    case WM_MOUSEWHEEL:
    {
        // The high-order word indicates the distance the wheel is rotated.
        // The low-order word is reserved and should be ignored.
        int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);

        if (zDelta)
            zDeltaQueue.push(zDelta);
        break;
    }
    case WM_INPUT: // manage raw input
    {
        RawData previouspos = mouse.GetRawData();
        UpdateRawInputData(lParam);
        RawData currentpos = mouse.GetRawData();
        rawDeltaQueue.push(currentpos - previouspos);

        break;
    }
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
            return 0;
        SetResizeInfo(ResizeInfo((UINT)LOWORD(lParam), (UINT)HIWORD(lParam)));
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    case WM_DPICHANGED:
        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DpiEnableScaleViewports)
        {
            //const int dpi = HIWORD(wParam);
            //printf("WM_DPICHANGED to %d (%.0f%%)\n", dpi, (float)dpi / 96.0f * 100.0f);
            const RECT* suggested_rect = (RECT*)lParam;
            ::SetWindowPos(hWnd, nullptr, suggested_rect->left, suggested_rect->top, suggested_rect->right - suggested_rect->left, suggested_rect->bottom - suggested_rect->top, SWP_NOZORDER | SWP_NOACTIVATE);
        }
        break;
    }
    return ::DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

std::optional<RawData> DirectXWindow::ReadRawDelta()
{
    if (!rawDeltaQueue.empty())
    {
        RawData delta = rawDeltaQueue.front();
        rawDeltaQueue.pop();
        return delta;
    }
    return {};
}

std::optional<int> DirectXWindow::ReadZDelta()
{
    if (!zDeltaQueue.empty())
    {
        int zDelta = zDeltaQueue.front();
        zDeltaQueue.pop();
        return zDelta;
    }
    return std::nullopt;
}

// USE RAW INPUT TO GET DELTA OF MOUSE

void DirectXWindow::CaptureCursor()
{
    captured = true;
    RECT rc;
    GetWindowRect(hWnd, &rc);
    SetCapture(hWnd);
    ClipCursor(&rc);
    ShowCursor(FALSE);
}

void DirectXWindow::ReleaseCursor()
{
    ClipCursor(NULL);
    ShowCursor(TRUE);
    ReleaseCapture();
    captured = false;
}

void DirectXWindow::UpdateRawInputData(const LPARAM& lParam)
{
    // ������� �������� � ������� rawinput data
    UINT required_size = 0U;
    GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &required_size, sizeof(RAWINPUTHEADER));
    BYTE* pByteCode = new BYTE[required_size];
    UINT bytes = GetRawInputData((HRAWINPUT)lParam, RID_INPUT, pByteCode, &required_size, sizeof(RAWINPUTHEADER));
    DOK_assert(bytes == required_size, L"��������� ������� ������� ��� rawinput � ���������� �� ���������");

    // �������������� ��� ��������� �� ��������� RAWINPUT
    PRAWINPUT pRawInput = (PRAWINPUT)pByteCode;

    if (pRawInput->header.dwType == RIM_TYPEMOUSE)
    {
        if (pRawInput->data.mouse.lLastX != 0 || pRawInput->data.mouse.lLastY != 0)
            mouse.OnDeltaRaw(pRawInput->data.mouse.lLastX, pRawInput->data.mouse.lLastY);
    }

    if (pByteCode) delete[] pByteCode;
}


