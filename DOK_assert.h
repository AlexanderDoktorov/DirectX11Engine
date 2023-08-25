#pragma once
#include <string>
#include <Windows.h>

void DOK_OnFailedAssert(const wchar_t* TEXT, UINT line,bool exit = true);

#define DOK_assert(expr, message) if(!(expr)) DOK_OnFailedAssert(message, __LINE__, true)

#define DOK_assert_noexit(expr, message) if(!(expr)) DOK_OnFailedAssert(message, __LINE__, false)