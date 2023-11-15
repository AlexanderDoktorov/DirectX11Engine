#pragma once
#include "Interfaces.h"
#include "DOK_DX11.h"
#include "DrawableBase.h"
#include "HPipelineElements.h"
#include "Sphere.h"

class SolidTexturedBall : public DrawableBase<SolidTexturedBall>, public IMovable, public IToString
{
public:
    SolidTexturedBall(Graphics& Gfx, float radius = 1.f) : r(radius)
    {
        if (!Initilized())
        {
            DynamicVertex::VertexLayout vertexLayout;
            vertexLayout.Append(DynamicVertex::VertexLayout::Position3D).Append(DynamicVertex::VertexLayout::Texture2D).Append(DynamicVertex::VertexLayout::Normal);
            auto model = Sphere::MakeTesselatedTexturedNormalized(vertexLayout, 40, 40);

            std::unique_ptr<VertexShaderCommon> VS = std::make_unique<VertexShaderCommon>(Gfx, L"shaders\\GeometryTexturedVS.cso");

            AddStaticBindable(std::make_unique<VertexBuffer>(Gfx, model.vertices));
            AddStaticBindable(std::make_unique<PixelShader>(Gfx, L"shaders\\GeometryTexturedPS.cso"));
            AddStaticBindable(std::make_unique<InputLayout>(Gfx, vertexLayout.GetD3DLayout(), VS.get()));
            AddStaticBindable(std::move(VS));
            AddStaticBindable(std::make_unique<IndexBuffer>(Gfx, model.indices));
            AddStaticBindable(std::make_unique<PixelShaderPictureTexture>(Gfx, L"G:\\Visual Studio Projects\\DirectX11LearnFromScrap\\Текстуры\\globus_texture.jpeg", 3U));
        }
        AddBindable(std::make_unique<TransformBuffer>(Gfx, *this));
        AddBindable(std::make_unique<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
    }

    void MakeSkeleton()
    {
        if (auto tp = QueryBindable<Topology>())
        {
            tp->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
        }
    }

    DirectX::XMMATRIX GetTransform() const noexcept override
    {
        return DOK_XMMatrixTranslation(world_position);
    }

    // IMovable
    using IMovable::SetPosition;
    virtual void SetPosition(float _x, float _y, float _z) override;
    virtual DirectX::XMFLOAT3 GetPosition() const noexcept override;

    // IToString
    const char* ToString() const noexcept override;

private:
    dx::XMFLOAT3 world_position = { 0.f, 0.f, 0.f };
    float r = 1.f;
};