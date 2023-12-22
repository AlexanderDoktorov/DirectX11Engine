#pragma once
#include <Windows.h>
#include <optional>
#include "WindowClass.h"

template <class T>
class BaseWindow
{
public:
	virtual ~BaseWindow() = default;
	BaseWindow() : hWnd(NULL) {}

	HWND GetWnd() const { return hWnd; }
	RECT GetWndRect() const noexcept 
	{
		RECT rect{};
		if (hWnd)
			GetClientRect(hWnd, &rect);
		return rect;
	}

	static std::optional<int> ProcessMessage() noexcept
	{
		MSG msg;
		while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				return (int)msg.wParam;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// return empty optional when not quitting app
		return {};
	}

	bool Create(
		PCWSTR lpWindowName,
		DWORD dwStyle,
		DWORD dwExStyle = 0,
		int x = CW_USEDEFAULT,
		int y = CW_USEDEFAULT,
		int nWidth = CW_USEDEFAULT,
		int nHeight = CW_USEDEFAULT,
		HWND hWndParent = 0,
		HMENU hMenu = 0
	)
	{
		if (!wclass.IsRegistered())
		{
			WNDCLASS wc = {};
			wc.lpszClassName = ClassName();
			wc.hInstance = GetModuleHandle(NULL);
			wc.lpfnWndProc = T::WindowProc;

			wclass.SetClass(wc);
			wclass.Register();
		}

		hWnd = CreateWindowEx(dwExStyle, ClassName(), lpWindowName, dwStyle, x, y, nWidth, nHeight, hWndParent, hMenu, GetModuleHandle(NULL), this);

		return (hWnd ? TRUE : FALSE);
	}

	void Show(int nCmdShow)
	{
		ShowWindow(hWnd, nCmdShow);
	}

	long GetWidth() const noexcept
	{
		RECT rc = RECT();
		GetClientRect(hWnd, &rc);
		return rc.right - rc.left;
	}

	long GetHeight() const noexcept
	{
		RECT rc = RECT();
		GetClientRect(hWnd, &rc);
		return rc.bottom - rc.top;
	}

	virtual LPCWSTR ClassName() const noexcept = 0;
	virtual LRESULT CALLBACK HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

protected:

	HWND hWnd;
	static WindowClass wclass;

	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if (uMsg == WM_NCCREATE)
		{
			CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
			T* wnd = reinterpret_cast<T*>(pCreate->lpCreateParams);
			SetWindowLongPtrW(hwnd, GWLP_USERDATA, (LONG_PTR)wnd); // устанавливаем класс T как пользовательские данные созданного окна

			wnd->hWnd = hwnd;
		}
		else
		{
			if (T* wnd = (T*)GetWindowLongPtrW(hwnd, GWLP_USERDATA))
			{
				return wnd->HandleMessage(uMsg, wParam, lParam);
			}
		}
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
};

template <class T>
WindowClass BaseWindow<T>::wclass {};

