#pragma once
#include <string>
#include <Windows.h>

void DOK_OnFailedAssert(const wchar_t* TEXT, UINT line);

#define DOK_assert(expr, message) if(!(expr)) DOK_OnFailedAssert(message, __LINE__)