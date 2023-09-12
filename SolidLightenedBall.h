#pragma once
#include "Interfaces.h"
#include "DOK_DX11.h"
#include "DrawableBase.h"
#include "HPipelineElements.h"
#include "Sphere.h"

class SolidLightenedBall : public DrawableBase<SolidLightenedBall>, public IColored, public IMovable, public IToString, public IObject
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
                dx::XMFLOAT2 tc;
                dx::XMFLOAT3 n;
            };

            auto model = Sphere::MakeTesselatedIndependentTexturedNormalized<Vertex>(20 , 20, 1);

            std::vector<D3D11_INPUT_ELEMENT_DESC> inputElementDesc =
            {
                { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
                { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
                { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
            };

            std::unique_ptr<VertexShaderCommon> VS = std::make_unique<VertexShaderCommon>(Gfx, L"GeometryTexturedVS.cso");

            AddStaticElement(std::make_unique<VertexBuffer>(Gfx, model.vertices));
            AddStaticElement(std::make_unique<PixelShader>(Gfx, L"GeometryTexturedPS.cso"));
            AddStaticElement(std::make_unique<InputLayout>(Gfx, inputElementDesc, VS.get()));
            AddStaticElement(std::move(VS));
            AddStaticElement(std::make_unique<IndexBuffer>(Gfx, model.indices));
            AddStaticElement(std::make_unique<PixelShaderPictureTexture>(Gfx, L"G:\\Visual Studio Projects\\DirectX11LearnFromScrap\\Текстуры\\globus_texture.jpeg", 3U));
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

    // IMovable
    using IMovable::SetPosition;
    virtual void SetPosition(float _x, float _y, float _z) override;
    virtual DirectX::XMFLOAT3 GetPosition() const noexcept override;

    // IToString
    const char* ToString() const noexcept override;

    // IColored
    DirectX::XMFLOAT4 GetColor() const noexcept override;
    void SetColor(dx::XMFLOAT4 new_color) noexcept override;

private:
    dx::XMFLOAT3 world_position = { 0.f, 0.f, 0.f };
    dx::XMFLOAT4 color = { 1.f,1.f,1.f,1.f };
};