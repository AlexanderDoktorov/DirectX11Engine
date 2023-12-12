#pragma once
#include "IShaderResourse.h"
#include "ISlot.h"
#include "IBindable.h"
#include "DOK_traits.h"

class ShaderResourseBase : public Slotted, virtual public IShaderResourse, public IBindable {};

template<class T>
struct ShaderResourse : public ShaderResourseBase {};

template<>
struct ShaderResourse<shader_type_vertex> : public ShaderResourseBase
{
	void Bind(Graphics& Gfx) noexcept override
	{
		ID3D11ShaderResourceView* pSRV = GetSRV();
		GetContext(Gfx)->VSSetShaderResources(GetBindSlot(), 1U, &pSRV);
	}
};


template<>
struct ShaderResourse<shader_type_pixel> : public ShaderResourseBase
{
	void Bind(Graphics& Gfx) noexcept override
	{
		ID3D11ShaderResourceView* pSRV = GetSRV();
		GetContext(Gfx)->PSSetShaderResources(GetBindSlot(), 1U, &pSRV);
	}
};


template<>
struct ShaderResourse<shader_type_compute> : public ShaderResourseBase
{
	void Bind(Graphics& Gfx) noexcept override
	{
		ID3D11ShaderResourceView* pSRV = GetSRV();
		GetContext(Gfx)->CSSetShaderResources(GetBindSlot(), 1U, &pSRV);
	}
};

template<>
struct ShaderResourse<shader_type_geometry> : public ShaderResourseBase
{
	void Bind(Graphics& Gfx) noexcept override
	{
		ID3D11ShaderResourceView* pSRV = GetSRV();
		GetContext(Gfx)->GSSetShaderResources(GetBindSlot(), 1U, &pSRV);
	}
};

template<>
struct ShaderResourse<shader_type_hull> : public ShaderResourseBase
{
	void Bind(Graphics& Gfx) noexcept override
	{
		ID3D11ShaderResourceView* pSRV = GetSRV();
		GetContext(Gfx)->HSSetShaderResources(GetBindSlot(), 1U, &pSRV);
	}
};

template<class... ShaderTypes>
struct ComboSResrouse : public ShaderResourse<ShaderTypes>...
{
	ComboSResrouse() = default;

	template<class ShaderType>
	void SetShaderBindSlot(UINT bindSlot) noexcept
	{
		ShaderResourse<ShaderType>::SetBindSlot(bindSlot);
	}

	virtual void Bind(Graphics& Gfx) noexcept override
	{
		(ShaderResourse<ShaderTypes>::Bind(Gfx), ...);
	}
	virtual ~ComboSResrouse() = default;
};