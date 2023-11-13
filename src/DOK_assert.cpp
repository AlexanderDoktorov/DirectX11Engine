#include "DOK_assert.h"

void DOK_OnFailedAssert(const wchar_t* TEXT, UINT line, bool exit)
{
	std::wstring wstr;
	wstr.append(L"Assert failed (LINE ");
	wstr.append(std::to_wstring(line) + L")");
	MessageBoxW(NULL, TEXT, wstr.c_str(), MB_OK);
	if(exit)
		_exit(-1);
	std::terminate();
}
