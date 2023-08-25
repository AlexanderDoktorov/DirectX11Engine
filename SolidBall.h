#pragma once
#include "DrawableBase.h"
#include "HPipelineElements.h"
#include "Sphere.h"

class SolidBall : public DrawableBase<SolidBall>, public IColored
{
public:
    SolidBall(Graphics& Gfx, dx::XMFLOAT4 solid_color = { 1.f,1.f,1.f,1.f })
    {
        SetColor(solid_color);
        if (!Initilized())
        {
            struct Vertex
            {
                dx::XMFLOAT3 pos;
                dx::XMFLOAT3 n;
            };

            auto model = Sphere::MakeTesselated<Vertex>(10U, 10U);

            std::vector<D3D11_INPUT_ELEMENT_DESC> inputElementDesc =
            {
                { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
            };

            std::unique_ptr<VertexShader> VS = std::make_unique<VertexShader>(Gfx, L"SolidVS.cso");
            auto ShBC = VS->GetBlob();

            AddStaticElement(std::make_unique<VertexBuffer>(Gfx, model.vertices));
            AddStaticElement(std::make_unique<PixelShader>(Gfx, L"SolidPS.cso"));
            AddStaticElement(std::move(VS));
            AddStaticElement(std::make_unique<InputLayout>(Gfx, inputElementDesc, ShBC));
            AddStaticElement(std::make_unique<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
            AddStaticElement(std::make_unique<IndexBuffer>(Gfx, model.indices));
        }
        AddElement(std::make_unique<TransformBuffer>(Gfx, *this));
        AddElement(std::make_unique<ColorBuffer>(Gfx, *this));
    }

    void MakeSkeleton()
    {
        Topology* topology = QueryBindable<Topology>();
        topology->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
    }

    virtual dx::XMMATRIX GetTransform() const noexcept override
    {
        return  dx::XMMatrixScaling(scale.x, scale.y, scale.z) * dx::XMMatrixTranslation(position.x, position.y, position.z);
    }

    virtual void Update(float dt) noexcept override
    {
        return;
    }

    void SetPosition(dx::XMFLOAT3 new_pos) noexcept
    {
        position = new_pos;
    }

    void SetScale(dx::XMFLOAT3 new_scale) noexcept
    {
        scale = new_scale;
    }

private:
    dx::XMFLOAT3 position = { 0.f, 0.f, 0.f };
    dx::XMFLOAT3 scale = { 1.f,1.f,1.f };
};