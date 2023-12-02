#pragma once
#include "ISlot.h"
#include "IBindable.h"
#include "RenderTexture.h"
#include "PictureTexture.h"

// Render target texuture for pixel shader
class RTTexturePS : public RenderTexture, public IBindable, public Slotted
{
public:
	RTTexturePS(Graphics& Gfx, DXGI_FORMAT textureFormat, UINT TextureHeight, UINT TextureWidth, UINT bindSlot)
		:
		RenderTexture(Gfx, textureFormat, TextureHeight, TextureWidth),
		Slotted(bindSlot)
	{

	}

	virtual void Bind(Graphics& Gfx) noexcept override;
};

// Texture2DArray for pixel shader
class TextureArrayPS : public IBindable, public Slotted
{
public:
	TextureArrayPS(Graphics& Gfx, UINT bindSlot) :
		Slotted(bindSlot)
	{

	}

	TextureArrayPS(Graphics& Gfx, const wchar_t* FileName,  UINT bindSlot, DirectX::WIC_LOADER_FLAGS loadFlags = DirectX::WIC_LOADER_DEFAULT) :
		Slotted(bindSlot)
	{
		PushBack(Gfx, FileName, loadFlags);
	}

	TextureArrayPS(Graphics& Gfx, const char* FileName,  UINT bindSlot, DirectX::WIC_LOADER_FLAGS loadFlags = DirectX::WIC_LOADER_DEFAULT) :
		Slotted(bindSlot)
	{
		PushBack(Gfx, FileName, loadFlags);
	}

	void PushBack(Graphics& Gfx, const wchar_t* FileName, DirectX::WIC_LOADER_FLAGS loadFlags = DirectX::WIC_LOADER_DEFAULT)
	{
		pictures.push_back(PictureTexture(Gfx, FileName, loadFlags));
	}

	void PushBack(Graphics& Gfx, const char* FileName, DirectX::WIC_LOADER_FLAGS loadFlags = DirectX::WIC_LOADER_DEFAULT)
	{
		pictures.push_back(PictureTexture(Gfx, FileName, loadFlags));
	}

	bool Empty() const noexcept
	{
		return pictures.empty();
	}

	virtual void Bind(Graphics& Gfx) noexcept override;
private:
	std::vector<PictureTexture> pictures;
};