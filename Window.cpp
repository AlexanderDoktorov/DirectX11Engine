#include "Window.h"
#include "ImGui\backend\imgui_impl_win32.h"
#include <windowsx.h>
#include <strsafe.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT Window::HandeMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
        return true;

    switch (uMsg)
    {
    case WM_KEYDOWN:
        if(wParam == 0x43) // C - Camera
        {
            if (!captured)
                CaptureCursor();
            else
                ReleaseCursor();

            break;
        }
        break;
    case WM_MOUSEMOVE:

        break;
    case WM_INPUT: // manage raw input
    {
        // сначала получаем в байткод rawinput data
        UINT required_size = 0U;
        GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &required_size, sizeof(RAWINPUTHEADER));
        BYTE* pByteCode = new BYTE[required_size]; 
        UINT bytes = GetRawInputData((HRAWINPUT)lParam, RID_INPUT, pByteCode, &required_size, sizeof(RAWINPUTHEADER));
        DOK_assert(bytes == required_size, L"требуемые размеры буффера для rawinput и полученные не совпадают");

        // интерпретируем как указатель на структуру RAWINPUT
        PRAWINPUT pRawInput = (PRAWINPUT)pByteCode;

        if (pRawInput->header.dwType == RIM_TYPEMOUSE)
        {
            // LOG
            TCHAR pszDest[100];
            size_t size = 100;

            StringCchPrintf(pszDest, size,
                TEXT("Mouse: lLastX=%i lLastY=%i \r\n"),

                pRawInput->data.mouse.lLastX,
                pRawInput->data.mouse.lLastY);
            OutputDebugString(pszDest);

            mouse.OnDeltaRaw(pRawInput->data.mouse.lLastX, pRawInput->data.mouse.lLastY)
        }

        if(pByteCode) delete[] pByteCode;


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

// USE RAW INPUT TO GET DELTA OF MOUSE

void Window::CaptureCursor()
{
    captured = true;
    RECT rc;
    GetWindowRect(hWnd, &rc);
    SetCapture(hWnd);
    ClipCursor(&rc);
    ShowCursor(FALSE);
}

void Window::ReleaseCursor()
{
    ClipCursor(NULL);
    ShowCursor(TRUE);
    ReleaseCapture();
    captured = false;
}


