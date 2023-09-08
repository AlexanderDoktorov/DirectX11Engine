#pragma once
#include "IPlacableItem.h"
#include "IToString.h"
#include "DrawableBase.h"
#include "HPipelineElements.h"
#include "Sphere.h"

class SolidBall : public DrawableBase<SolidBall>, public IColored, public IPlacableItem, public IToString
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
                { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
                { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
            };

            std::unique_ptr<VertexShader> VS = std::make_unique<VertexShader>(Gfx, L"GeometryVS.cso");
            auto ShBC = VS->GetBlob();

            AddStaticElement(std::make_unique<VertexBuffer>(Gfx, model.vertices));
            AddStaticElement(std::make_unique<PixelShader>(Gfx, L"GeometryPS.cso"));
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
        SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
    }

    DirectX::XMMATRIX GetTransform() const noexcept override
    {
        return DOK_XMMatrixTranslation(world_position);
    }

    virtual void Update(float dt) noexcept override
    {
        return;
    }

    void SetScale(dx::XMFLOAT3 new_scale) noexcept
    {
        scale = new_scale;
    }

    // IPlacableItem
    void SetWorldPosition(const dx::XMFLOAT3& new_Wpos) override;
    dx::XMFLOAT3 GetWorldPosition() const noexcept override;

    // IToString
    const char* ToString() const noexcept override;

    // IColored
    DirectX::XMFLOAT4 GetColor() const noexcept override;
    void SetColor(dx::XMFLOAT4 new_color) noexcept override;

private:
    dx::XMFLOAT3 scale = { 1.f,1.f,1.f };
    dx::XMFLOAT3 world_position = { 0.f,0.f,0.f };
    dx::XMFLOAT4 color = { 1.f, 1.f, 1.f, 1.f };
};