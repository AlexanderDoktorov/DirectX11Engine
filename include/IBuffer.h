#pragma once
#include <d3d11.h>

struct IBuffer
{
	virtual ID3D11Buffer* GetBuffer() const noexcept = 0;
};