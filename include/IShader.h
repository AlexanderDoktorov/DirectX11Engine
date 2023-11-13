#pragma once
#include "IBindable.h"
#include "IBlob.h"

class IShader : virtual public IBindable, virtual  public IBlob
{
public:
	virtual ~IShader() = default;
	virtual void SetShaderResourses(Graphics& Gfx, UINT start_slot, std::vector<ID3D11ShaderResourceView*> srvs) noexcept = 0;
	virtual void SetConstantBuffers(Graphics& Gfx, UINT start_slot, std::vector<ID3D11Buffer*> constant_buffers) noexcept = 0;
};