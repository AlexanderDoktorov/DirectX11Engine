#include "DOK_assert.h"

void DOK_OnFailedAssert(const wchar_t* TEXT, UINT line)
{
	std::wstring wstr;
	wstr.append(L"Assert failed (LINE ");
	wstr.append(std::to_wstring(line) + L")");
	MessageBoxW(NULL, TEXT, wstr.c_str(), MB_OK);
	_exit(-1);
}
