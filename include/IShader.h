#pragma once
#include "IBindable.h"
#include "IBlob.h"
#include <vector>

struct IShader : public IBindable, public IUnbindable, public IBlob
{
	virtual ~IShader() = default;
	virtual void LoadFromFile(Graphics& Gfx, const wchar_t* FileName) = 0;
	virtual void SetShaderResourses(Graphics& Gfx, UINT start_slot, std::vector<ID3D11ShaderResourceView*> srvs) noexcept = 0;
	virtual void SetConstantBuffers(Graphics& Gfx, UINT start_slot, std::vector<ID3D11Buffer*> constant_buffers) noexcept = 0;
	
	template<class T>
	T* GetShader() const noexcept
	{
		return static_cast<T*>(GetShaderImpl());
	}

private:
	virtual void* GetShaderImpl() const noexcept = 0;
};