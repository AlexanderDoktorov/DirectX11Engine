#pragma once
#include "IBindable.h"

class IShader : public IBindable
{
public:
	virtual void SetShaderResourses(Graphics& Gfx, UINT start_slot, std::vector<ID3D11ShaderResourceView*> srvs) noexcept = 0;
	virtual void SetConstantBuffers(Graphics& Gfx, UINT start_slot, std::vector<ID3D11Buffer*> constant_buffers) noexcept = 0;
};