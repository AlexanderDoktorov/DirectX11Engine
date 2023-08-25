#pragma once
#include "DrawableBase.h"
#include "HPipelineElements.h"
#include "Sphere.h"

class SolidLightenedBall : public DrawableBase<SolidLightenedBall>, public IColored
{
public:
    SolidLightenedBall(Graphics& Gfx, dx::XMFLOAT4 color =  { 1.f, 1.f, 1.f, 1.f })
    {
        SetColor(color);
        if (!Initilized())
        {
            struct Vertex
            {
                dx::XMFLOAT3 pos;
                dx::XMFLOAT3 n;
            };

            auto model = Sphere::MakeTesselated<Vertex>(20U, 20U); 
            model.SetNormalsIndependentFlat();

            std::vector<D3D11_INPUT_ELEMENT_DESC> inputElementDesc =
            {
                { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
                { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
            };

            std::unique_ptr<VertexShader> VS = std::make_unique<VertexShader>(Gfx, L"SolidLitVS.cso");
            auto ShBC = VS->GetBlob();

            AddStaticElement(std::make_unique<VertexBuffer>(Gfx, model.vertices));
            AddStaticElement(std::make_unique<PixelShader>(Gfx, L"SolidLitPS.cso"));
            AddStaticElement(std::move(VS));
            AddStaticElement(std::make_unique<InputLayout>(Gfx, inputElementDesc, ShBC));
            AddStaticElement(std::make_unique<IndexBuffer>(Gfx, model.indices));
        }
        AddElement(std::make_unique<TransformBuffer>(Gfx, *this));
        AddElement(std::make_unique<ColorBuffer>(Gfx, *this));
        AddElement(std::make_unique<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
    }

    void MakeSkeleton()
    {
        if (Topology* topology = QueryBindable<Topology>())
        {
            topology->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
        }
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

private:
    dx::XMFLOAT3 position = { 0.f, 0.f, 0.f };
    dx::XMFLOAT3 scale = { 1.f,1.f,1.f };
};