#pragma once
#include "ISlot.h"
#include "IBindable.h"
#include "RenderTexture.h"
#include "PictureTexture.h"

class PixelShaderRenderTexture : public RenderTexture, public IBindable, public ISlot
{
public:
	PixelShaderRenderTexture(Graphics& Gfx, DXGI_FORMAT textureFormat, UINT TextureHeight, UINT TextureWidth, UINT bindSlot)
		:
		RenderTexture(Gfx, textureFormat, TextureHeight, TextureWidth),
		bindSlot(bindSlot)
	{

	}
	
	// ISlot
	void SetBindSlot(UINT slot) noexcept override;
	UINT GetBindSlot() const noexcept override;
	
	virtual void Bind(Graphics& Gfx) noexcept override;
private:
	UINT bindSlot = 0U;
};

class PixelShaderPictureTexture : public PictureTexture, public IBindable, public ISlot
{
public:
	PixelShaderPictureTexture(Graphics& Gfx, const wchar_t* FileName, UINT bindSlot)
		:
		PictureTexture(Gfx, FileName),
		bindSlot(bindSlot)
	{

	}

	// ISlot
	void SetBindSlot(UINT slot) noexcept override;
	UINT GetBindSlot() const noexcept override;

	virtual void Bind(Graphics& Gfx) noexcept override;

private:
	UINT bindSlot = 0U;
};