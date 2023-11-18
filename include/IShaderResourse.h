#pragma once
#include <d3d11.h>

struct IShaderResourse
{
	virtual ID3D11ShaderResourceView* GetSRV() const noexcept = 0;
};