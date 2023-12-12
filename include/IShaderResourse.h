#pragma once
#include <d3d11.h>

struct IShaderResourse
{
	IShaderResourse() = default;
	virtual ID3D11ShaderResourceView* GetSRV() const noexcept = 0;
};
