#pragma once
#include <wrl.h>
#include <d3d11.h>
#include <vector>
#include "Vertex.h"
#include "DOK_assert.h"
#include "IShader.h"
#include "BindableSystem.h"


class InputLayout : public IBindable, public IGetID
{
public:
	InputLayout(Graphics& Gfx, const DynamicVertex::VertexLayout& vl, IShader* pShader);

	void Bind(Graphics& Gfx) noexcept override;
	std::string GetID() const noexcept override;
	static std::string GenerateID(const DynamicVertex::VertexLayout& vl, [[maybe_unused]] IShader* = nullptr);
	static std::shared_ptr<InputLayout> Resolve(Graphics& Gfx,const DynamicVertex::VertexLayout& vl, IShader* pShader);
private:
	DynamicVertex::VertexLayout layout;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> p_IL;
};