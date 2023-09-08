#pragma once
#include "ITexture.h"
#include "GraphicsChild.h"

class RenderTexture : public ITexture, virtual public GraphicsChild
{
public:
	RenderTexture(Graphics& Gfx, DXGI_FORMAT textureFormat, UINT TextureHeight, UINT TextureWidth);
	RenderTexture(Graphics& Gfx, const RenderTexture& otherTexture);

	void CreateRenderTexture(Graphics& Gfx, DXGI_FORMAT textureFormat, UINT TextureHeight, UINT TextureWidth);
	void Reset();
	void Resize(Graphics& Gfx, UINT TextureHeight, UINT TextureWidth);

	ID3D11Texture2D*			GetTexture() const noexcept override;
	ID3D11ShaderResourceView*	GetSRV() const noexcept override;
	D3D11_TEXTURE2D_DESC		GetDesc() const noexcept override;

	~RenderTexture() = default;
private:
	Microsoft::WRL::ComPtr<ID3D11Texture2D> p_Texture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> p_ShaderResourseView;
};