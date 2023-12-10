#pragma once
#include "TextureBase.h"

class RenderTexture : public TextureBase
{
public:
	RenderTexture() = default;
	RenderTexture(Graphics& Gfx, DXGI_FORMAT textureFormat, UINT TextureHeight, UINT TextureWidth);
	RenderTexture(Graphics& Gfx, const RenderTexture& otherTexture);

	void CreateRenderTexture(Graphics& Gfx, DXGI_FORMAT textureFormat, UINT TextureHeight, UINT TextureWidth);
	void Reset();
	void Resize(Graphics& Gfx, UINT TextureHeight, UINT TextureWidth);
};