#pragma once
#include "DrawableBase.h"
#include "HPipelineElements.h"
#include "Interfaces.h"
#include "Sampler.h"
#include "Cube.h"
#include "Vertex.h"

// Smooth nomals
#include "G:\Visual Studio Projects\Template_metaprogramming\AverageNormalsCalculation.h"

class Bar : public DrawableBase<Bar>, public IMovable, public IScalable, public IColored
{
public:
	Bar(Graphics& Gfx, float length = 4.f, float height = 1.f, float width = 1.f) : scale_x(length), scale_y(height), scale_z(width)
	{
		if (!Initilized())
		{
			Vertex::VertexLayout layout{};
			layout.Append(Vertex::VertexLayout::Position3D).Append(Vertex::VertexLayout::Normal).Append(Vertex::VertexLayout::Texture2D);
			float side = 0.5f;

			Vertex::VertexBuffer vb{ std::move( layout ), 8U };

			vb[0].Attr<Vertex::VertexLayout::Position3D>() = dx::XMFLOAT3({ -side,-side,-side	});// 0 near side
			vb[1].Attr<Vertex::VertexLayout::Position3D>() = dx::XMFLOAT3({ side,-side,-side	});// 1
			vb[2].Attr<Vertex::VertexLayout::Position3D>() = dx::XMFLOAT3({ -side,side,-side	});// 2
			vb[3].Attr<Vertex::VertexLayout::Position3D>() = dx::XMFLOAT3({ side,side,-side		});// 3
			vb[4].Attr<Vertex::VertexLayout::Position3D>() = dx::XMFLOAT3({ -side,-side,side	});// 4 far side
			vb[5].Attr<Vertex::VertexLayout::Position3D>() = dx::XMFLOAT3({ side,-side,side		});// 5
			vb[6].Attr<Vertex::VertexLayout::Position3D>() = dx::XMFLOAT3({ -side,side,side		});// 6
			vb[7].Attr<Vertex::VertexLayout::Position3D>() = dx::XMFLOAT3({ side,side,side		});// 7

			std::vector<unsigned short> indxs =
			{
				0,2,1, 2,3,1,
				1,3,5, 3,7,5,
				2,6,3, 3,6,7,
				4,5,7, 4,7,6,
				0,4,2, 2,4,6,
				0,1,4, 1,5,4
			};

			std::vector<FaceDesc> faces =
			{
				FaceDesc(0,2,1), FaceDesc(2,3,1),
				FaceDesc(1,3,5), FaceDesc(3,7,5),
				FaceDesc(2,6,3), FaceDesc(3,6,7),
				FaceDesc(4,5,7), FaceDesc(4,7,6),
				FaceDesc(0,4,2), FaceDesc(2,4,6),
				FaceDesc(0,1,4), FaceDesc(1,5,4)
			};

			std::vector<dx::XMFLOAT3> verts =
			{
				vb[0].Attr<Vertex::VertexLayout::Position3D>(),
				vb[1].Attr<Vertex::VertexLayout::Position3D>(),
				vb[2].Attr<Vertex::VertexLayout::Position3D>(),
				vb[3].Attr<Vertex::VertexLayout::Position3D>(),
				vb[4].Attr<Vertex::VertexLayout::Position3D>(),
				vb[5].Attr<Vertex::VertexLayout::Position3D>(),
				vb[6].Attr<Vertex::VertexLayout::Position3D>(),
				vb[7].Attr<Vertex::VertexLayout::Position3D>(),
			};

			calc_mesh_normals(verts, faces);

			vb[0].Attr<Vertex::VertexLayout::Normal>() = dx::XMFLOAT3({ -side,-side,-side	});// 0 near side
			vb[1].Attr<Vertex::VertexLayout::Normal>() = dx::XMFLOAT3({ side,-side,-side	});// 1
			vb[2].Attr<Vertex::VertexLayout::Normal>() = dx::XMFLOAT3({ -side,side,-side	});// 2
			vb[3].Attr<Vertex::VertexLayout::Normal>() = dx::XMFLOAT3({ side,side,-side		});// 3
			vb[4].Attr<Vertex::VertexLayout::Normal>() = dx::XMFLOAT3({ -side,-side,side	});// 4 far side
			vb[5].Attr<Vertex::VertexLayout::Normal>() = dx::XMFLOAT3({ side,-side,side		});// 5
			vb[6].Attr<Vertex::VertexLayout::Normal>() = dx::XMFLOAT3({ -side,side,side		});// 6
			vb[7].Attr<Vertex::VertexLayout::Normal>() = dx::XMFLOAT3({ side,side,side		});// 7

			IndexedTriangleList model(vb, indxs);

			std::unique_ptr<VertexShaderCommon> VS = std::make_unique<VertexShaderCommon>(Gfx, L"GeometryVS.cso");

			AddStaticBindable(std::make_unique<VertexBuffer>(Gfx, model.vertices));
			AddStaticBindable(std::make_unique<PixelShaderCommon>(Gfx, L"GeometryPS.cso"));
			AddStaticBindable(std::make_unique<InputLayout>(Gfx, model.vertices.GetLayout().GetD3DLayout(), VS.get()));
			AddStaticBindable(std::move(VS));
			AddStaticBindable(std::make_unique<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
			AddStaticBindable(std::make_unique<IndexBuffer>(Gfx, model.indices));
			AddStaticBindable(std::make_unique<Sampler>(Gfx));
			//AddStaticBindable(std::make_unique<PixelShaderPictureTexture>(Gfx, L"G:\\Visual Studio Projects\\DirectX11LearnFromScrap\\Текстуры\\Текстура_квадрата.png", 3U));
		}
		AddBindable(std::make_unique<TransformBuffer>(Gfx, *this));
		AddBindable(std::make_unique<ColorBuffer>(Gfx, *this));
	}

	virtual DirectX::XMMATRIX	GetTransform()				const noexcept override
	{
		return DirectX::XMMatrixScaling(scale_x, scale_y, scale_z) * DirectX::XMMatrixTranslation(x, y, z);
	}

	// IMovable
	virtual void SetPosition(float _x, float _y, float _z) override;
	virtual DirectX::XMFLOAT3 GetPosition() const noexcept override;

	// IScalable
	virtual void SetScale(const float& scale_x_new, const float& scale_y_new, const float& scale_z_new) override;
	virtual dx::XMFLOAT3 GetScale() const noexcept override;

	// IColored
	virtual DirectX::XMFLOAT4 GetColor()	const noexcept override;
	virtual void SetColor(dx::XMFLOAT4 new_color) noexcept override;

private:
	float x = 0.f;
	float y = 0.f;
	float z = 0.f;

	float scale_x = 1.f;
	float scale_y = 1.f;
	float scale_z = 1.f;

	DirectX::XMFLOAT4 color { 1.f,0.f,1.f,1.f };
};