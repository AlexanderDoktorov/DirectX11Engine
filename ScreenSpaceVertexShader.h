#pragma once
#include "VertexShader.h"
#include "VertexBuffer.h"
#include "InputLayout.h"
#include "VertexShaderCommon.h"

class ScreenSpaceVertexShader : public VertexShaderCommon
{
public:
	ScreenSpaceVertexShader(Graphics& Gfx) : VertexShaderCommon(Gfx, L"ScreenSpaceVS.cso")
	{
		struct ScreenVertex
		{
			dx::XMFLOAT2 screen_pos;
			dx::XMFLOAT2 textcoord;
		};

		std::vector<D3D11_INPUT_ELEMENT_DESC> inputElementDesc =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{ "TEXTCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		std::vector<ScreenVertex> verticies =
		{
			{ dx::XMFLOAT2(-1.0f, 1.0f), dx::XMFLOAT2(0.f,0.f) },
			{ dx::XMFLOAT2(3.0f, 1.0f), dx::XMFLOAT2(2.0f, 0.0f) },
			{ dx::XMFLOAT2(-1.0f, -3.0f), dx::XMFLOAT2(0.0f, 2.0f) }
		};

		CreateInputLayout(Gfx, inputElementDesc);
		CreateVertexBuffer(Gfx, verticies);
	}
};