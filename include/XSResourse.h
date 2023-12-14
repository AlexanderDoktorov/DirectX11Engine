#pragma once
#include "IShaderResourse.h"
#include "IBuffer.h"
#include "ISlot.h"
#include "IBindable.h"

struct shader_type {};
struct shader_type_pixel	: public shader_type {};
struct shader_type_vertex	: public shader_type {};
struct shader_type_hull		: public shader_type {};
struct shader_type_compute	: public shader_type {};
struct shader_type_geometry : public shader_type {};

#pragma region combo_shader_resourse

class ShaderResourseBase : public Slotted, virtual public IShaderResourse, public IBindable {};

template<class T>
struct ShaderResourse;

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

#pragma endregion combo_shader_resourse

#pragma region combo_buffer

struct IComboBuffer : public IBuffer, public IBindable, protected Slotted {};

template<class T>
struct ComboBufferBase;

template<>
struct ComboBufferBase<shader_type_pixel> : public IComboBuffer
{
	void Bind(Graphics& Gfx) noexcept override
	{
		ID3D11Buffer* pBuffer = GetBuffer();
		GetContext(Gfx)->PSSetConstantBuffers(GetBindSlot(), 1U, &pBuffer);
	}
};

template<>
struct ComboBufferBase<shader_type_vertex> : public IComboBuffer
{
	void Bind(Graphics& Gfx) noexcept override
	{
		ID3D11Buffer* pBuffer = GetBuffer();
		GetContext(Gfx)->VSSetConstantBuffers(GetBindSlot(), 1U, &pBuffer);
	}
};

template<>
struct ComboBufferBase<shader_type_hull> : public IComboBuffer
{
	void Bind(Graphics& Gfx) noexcept override
	{
		ID3D11Buffer* pBuffer = GetBuffer();
		GetContext(Gfx)->HSSetConstantBuffers(GetBindSlot(), 1U, &pBuffer);
	}
};

template<>
struct ComboBufferBase<shader_type_compute> : public IComboBuffer
{
	void Bind(Graphics& Gfx) noexcept override
	{
		ID3D11Buffer* pBuffer = GetBuffer();
		GetContext(Gfx)->CSSetConstantBuffers(GetBindSlot(), 1U, &pBuffer);
	}
};

template<>
struct ComboBufferBase<shader_type_geometry> : public IComboBuffer
{
	void Bind(Graphics& Gfx) noexcept override
	{
		ID3D11Buffer* pBuffer = GetBuffer();
		GetContext(Gfx)->GSSetConstantBuffers(GetBindSlot(), 1U, &pBuffer);
	}
};

template<class... ShaderTypes>
struct ComboBuffer : public ComboBufferBase<ShaderTypes>...
{
	ComboBuffer() = default;
	template<class ShaderType>
	void SetShaderBindSlot(UINT bindSlot) noexcept
	{
		ComboBufferBase<ShaderType>::SetBindSlot(bindSlot);
	}

	template<class ShaderType>
	UINT GetShaderBindSlot() noexcept
	{
		return ComboBufferBase<ShaderType>::GetBindSlot();
	}

	void SetBindSlot(UINT slot) noexcept override
	{
		(ComboBufferBase<ShaderTypes>::SetBindSlot(slot), ...);
	}

	virtual void Bind(Graphics& Gfx) noexcept override
	{
		(ComboBufferBase<ShaderTypes>::Bind(Gfx), ...);
	}
	virtual ~ComboBuffer() = default;
};

#pragma endregion combo_buffer