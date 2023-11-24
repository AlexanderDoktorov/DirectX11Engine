#pragma once
#include "ISlot.h"
#include "IBindable.h"
#include "RenderTexture.h"
#include "PictureTexture.h"

class PixelShaderRenderTexture : public RenderTexture, public IBindable, public Slotted
{
public:
	PixelShaderRenderTexture(Graphics& Gfx, DXGI_FORMAT textureFormat, UINT TextureHeight, UINT TextureWidth, UINT bindSlot)
		:
		RenderTexture(Gfx, textureFormat, TextureHeight, TextureWidth),
		Slotted(bindSlot)
	{

	}

	virtual void Bind(Graphics& Gfx) noexcept override;
};

class PixelShaderPictureTexture : public PictureTexture, public IBindable, public Slotted
{
public:
	PixelShaderPictureTexture(Graphics& Gfx, const wchar_t* FileName, UINT bindSlot)
		:
		PictureTexture(Gfx, FileName),
		Slotted(bindSlot)
	{

	}

	PixelShaderPictureTexture(Graphics& Gfx, const char* FileName, UINT bindSlot)
		:
		PictureTexture(Gfx, FileName),
		Slotted(bindSlot)
	{

	}

	virtual void Bind(Graphics& Gfx) noexcept override;
};