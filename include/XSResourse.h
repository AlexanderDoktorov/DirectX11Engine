#pragma once
#include "IShaderResourse.h"
#include "ISlot.h"
#include "IBindable.h"

struct ShaderResourse : public Slotted, public IShaderResourse, public IBindable
{
	ShaderResourse() = default;
	ShaderResourse(UINT bindSlot);
};

struct PSResourse : public ShaderResourse
{
	virtual void Bind(Graphics& Gfx) noexcept override;
};

struct VSResourse : public ShaderResourse
{
	virtual void Bind(Graphics& Gfx) noexcept override;
};

struct HSResourse : public ShaderResourse
{
	virtual void Bind(Graphics& Gfx) noexcept override;
};

struct CSResourse : public ShaderResourse
{
	virtual void Bind(Graphics& Gfx) noexcept override;
};

template<class... XSResourses>
struct ComboSResrouse : public XSResourses...
{
	virtual void Bind(Graphics& Gfx) noexcept override
	{
		(void)(XSResourses::Bind(Gfx), ...);
	}
};