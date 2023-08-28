﻿#pragma once
#include "IPlacableItem.h"
#include "IToString.h"
#include "DrawableBase.h"
#include "HPipelineElements.h"
#include "Sphere.h"

class SolidLightenedBall : public DrawableBase<SolidLightenedBall>, public IColored, public IPlacableItem, public IToString
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

    // IPlacableItem
    virtual void SetWorldPosition(const dx::XMFLOAT3& new_Wpos) override;
    virtual dx::XMFLOAT3 GetWorldPosition() const noexcept override;

    // IToString
    const char* ToString() const noexcept override;

private:
    dx::XMFLOAT3 world_position = { 0.f, 0.f, 0.f };
};