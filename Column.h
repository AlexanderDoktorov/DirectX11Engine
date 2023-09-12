#pragma once
#include "DrawableBase.h"
#include "HPipelineElements.h"
#include "Graphics.h"
#include "ColorBuffer.h"
#include "Shift.h"

namespace dx = DirectX;

struct rectangle
{
	dx::XMFLOAT3 left_bottom;
	dx::XMFLOAT3 right_bottom;
	dx::XMFLOAT3 right_top;
	dx::XMFLOAT3 left_top;

	rectangle(const dx::XMFLOAT3& left_bottom, const dx::XMFLOAT3& right_bottom, const dx::XMFLOAT3& right_top, const dx::XMFLOAT3& left_top)
		: left_bottom(left_bottom), right_bottom(right_bottom), left_top(left_top), right_top(right_top)
	{
	}

	rectangle() = default;
};

class Column : public DrawableBase<Column>, public IColored
{
public:
	static void Make(Graphics& Gfx, const dx::XMFLOAT3& left_bottom_pos, const float& height = 0.5f, const float& width = 0.5f)
	{
		if (!Initilized())
		{
			dx::XMFLOAT3 buff = left_bottom_pos;
			std::vector<dx::XMFLOAT3> verts =
			{
				left_bottom_pos,
				Shift(buff, width, SHIFT_DIR_RIGHT),
				Shift(buff, height, SHIFT_DIR_UP),
				Shift(buff, width, SHIFT_DIR_LEFT)
			};

			ColumnRect = rectangle(verts[0], verts[1], verts[2], verts[3]);

			std::vector<unsigned short> indicies =
			{
				2, 1, 0,
				2, 0, 3
			};

			std::vector<D3D11_INPUT_ELEMENT_DESC> inputElementDesc =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
			};

			std::unique_ptr<VertexShaderCommon> VS = std::make_unique<VertexShaderCommon>(Gfx, L"DefaultVS.cso");

			AddStaticBindable(std::make_unique<VertexBuffer>(Gfx, verts));
			AddStaticBindable(std::make_unique<PixelShader>(Gfx, L"SolidPS.cso"));
			AddStaticBindable(std::make_unique<InputLayout>(Gfx, inputElementDesc, VS.get()));
			AddStaticBindable(std::move(VS));
			AddStaticBindable(std::make_unique<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
			AddStaticBindable(std::make_unique<IndexBuffer>(Gfx, indicies));
		}
	}

	Column(Graphics& Gfx)
	{
		DOK_assert(Initilized(), L"Make sure you made template of column with Column(Gfx, left_bottom_pos, height, width");

		AddBindable(std::make_unique<ColorBuffer>(Gfx, *this));
		AddBindable(std::make_unique<TransformBuffer>(Gfx, *this));
	}

	Column(const Column& other) = default;

	virtual DirectX::XMMATRIX GetTransform() const noexcept
	{
		auto scale_matrix = DirectX::XMMatrixScaling(scale_x, scale_y, scale_z) * DirectX::XMMatrixTranslation( GetInitialRect().left_bottom.x * (1.f - scale_x), GetInitialRect().left_bottom.y * (1.f - scale_y), 0.f);
		return   scale_matrix * DirectX::XMMatrixTranslation(offset_x, 0.f, 0.f);
	}

	void Update(float dt) noexcept
	{
		return;
	}

	static rectangle	GetInitialRect() noexcept;
	static float		GetInitialWidth() noexcept;
	static float		GetInitialHeight() noexcept;
	bool				operator>(const Column& other)
	{
		return scale_y > other.scale_y;
	}
	DirectX::XMFLOAT4	GetColor() const noexcept override { return color; }

	void SetColor(dx::XMFLOAT4 new_color) noexcept
	{
		color = new_color;
	}

	void SetScaleFactorY(float sc_y) { scale_y = sc_y; }
	void SetScaleFactorX(float sc_x) { scale_x = sc_x; }
	void SetX(float x) { offset_x = x; }

private:

	//position
	float offset_x = 0.f;

	//scale 
	float scale_x = 1.f;
	float scale_y = 1.f;
	float scale_z = 1.f;

	// Color
	dx::XMFLOAT4 color = { 1.f,1.f,1.f,1.f };

	static rectangle ColumnRect;
};