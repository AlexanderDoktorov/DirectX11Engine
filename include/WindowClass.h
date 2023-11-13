#pragma once

#include <Windows.h>

class WindowClass
{
public:
	WindowClass(const WNDCLASS& wc) : window_class(wc), is_registered(false) { }
	WindowClass() : window_class(), is_registered(false) { }

	void SetClass(const WNDCLASS& wc) noexcept
	{
		window_class = wc;
	}

	void Register() noexcept
	{
		if (!is_registered)
		{
			is_registered = true;
			RegisterClass(&window_class);
		}
	}
	
	bool IsRegistered() const noexcept
	{
		return is_registered;
	}

	~WindowClass() = default;

private:
	WNDCLASS window_class;
	bool is_registered;
};