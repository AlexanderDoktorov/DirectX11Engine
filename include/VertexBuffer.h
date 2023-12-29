#pragma once
#include "Vertex.h"
#include "hrException.h"
#include "BindableSystem.h"
#include <wrl.h>

class VertexBuffer : public IBindable
{
public:
	VertexBuffer( Graphics& gfx, std::string buff_tag, const DynamicVertex::VertexBuffer& vbuf );

	static std::string GenerateID(std::string buff_tag, const DynamicVertex::VertexBuffer& vbuf) noexcept;
	static std::shared_ptr<VertexBuffer> Resolve(Graphics& gfx, std::string buff_tag, const DynamicVertex::VertexBuffer& vbuf) noexcept;

	void Bind( Graphics& gfx ) noexcept override;

protected:
	UINT stride;
	std::string v_Tag;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
};
