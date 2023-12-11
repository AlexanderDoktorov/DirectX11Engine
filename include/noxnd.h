#pragma once

#ifndef noxnd
#ifdef _DEBUG
#define noxnd
#else
#define noxnd noexcept
#endif // _DEBUG
#endif

