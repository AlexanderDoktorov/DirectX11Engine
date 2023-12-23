#pragma once
#include "baseException.h"

#include <string>
#include <Windows.h>

#include <comdef.h>
#include <iostream>
#include <sstream>

inline static std::string utf8_encode(const std::wstring &wstr)
{
	if( wstr.empty() ) 
		return std::string();
	int size_needed = WideCharToMultiByte(CP_ACP , 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
	std::string strTo( size_needed, 0 );
	WideCharToMultiByte                  (CP_ACP , 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
	return strTo;
}

class hrException : public baseException
{
public:
	hrException(int codeLine, const char* codeFileName, HRESULT hr) 
		:
		baseException(codeLine, codeFileName),
		err(hr)
	{
		
	}

	std::string what() const noexcept override
	{
		std::stringstream ss;
		ss << "Failed with HRESULT: 0x" << std::hex << err.Error();
		ss << " Info: " << (err.ErrorMessage() ? utf8_encode(err.ErrorMessage()) : "could not find a description for error.") << std::endl;
		ss << baseException::what() << std::endl;
		return ss.str().c_str();
	}

	~hrException() = default;

private:
	_com_error err;
};

#define CHECK_EXPR_HR(expression) \
hr = expression; \
if(FAILED(hr)) \
{ \
	throw hrException(__LINE__, __FILE__, (HRESULT)hr); \
}

#define CHECK_EXPR_DEFINE_HR(expression) \
HRESULT hr = expression; \
if(FAILED(hr)) \
{ \
	throw hrException(__LINE__, __FILE__, (HRESULT)hr); \
}

#define CHECK_HR(hr) \
if(FAILED(hr)) \
{ \
	throw hrException(__LINE__, __FILE__, (HRESULT)hr); \
}