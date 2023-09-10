#pragma once
#include <d3d11.h>

class IBlob
{
public:
	virtual ~IBlob() = default;
	virtual ID3DBlob* GetBlob() noexcept = 0;
};