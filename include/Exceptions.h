#pragma once

#include <exception>
#include <string>
#include <Windows.h>
#include <Winerror.h>

#include <d3dcompiler.h>
#include <comdef.h>

class hrException
{
public:
	hrException(HRESULT hr) : err(hr)
	{

	}

	const wchar_t* what() const noexcept
	{
		return err.ErrorMessage() ? err.ErrorMessage() : TEXT("[Could not find a description for error.]\n");
	}

	~hrException() = default;

private:
	_com_error err;
};

class meshException
{
public:
	meshException(std::string note) : note(note)
	{

	}

	const char* what() const noexcept
	{
		return note.c_str() ? ("meshException occured : " + note + "\n").c_str() : "[Could not find a description for error.]\n";
	}

	~meshException() = default;

private:
	std::string note;
};

#define CHECK_HR(hr)  if(FAILED(hr)) throw hrException((HRESULT)hr)