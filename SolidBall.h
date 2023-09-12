#pragma once
#include "DOK_DX11.h"
#include "DrawableBase.h"
#include "HPipelineElements.h"
#include "Interfaces.h"
#include "Sphere.h"

class SolidBall : public DrawableBase<SolidBall>, virtual public IColored, virtual public IMovable, virtual public IToString, virtual public IObject
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
                dx::XMFLOAT2 tc;
                dx::XMFLOAT3 n;
            };

            auto model = Sphere::MakeTesselatedIndependentTexturedNormalized<Vertex>(40 , 40, 1);

            std::vector<D3D11_INPUT_ELEMENT_DESC> inputElementDesc =
            {
                { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
                { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
                { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
            };

            std::unique_ptr<VertexShaderCommon> VS = std::make_unique<VertexShaderCommon>(Gfx, L"GeometryTexturedVS.cso");

            AddStaticBindable(std::make_unique<VertexBuffer>(Gfx, model.vertices));
            AddStaticBindable(std::make_unique<PixelShaderCommon>(Gfx, L"GeometryTexturedPS.cso"));
            AddStaticBindable(std::make_unique<InputLayout>(Gfx, inputElementDesc, VS.get()));
            AddStaticBindable(std::move(VS));
            AddStaticBindable(std::make_unique<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
            AddStaticBindable(std::make_unique<IndexBuffer>(Gfx, model.indices));
            AddStaticBindable(std::make_unique<PixelShaderPictureTexture>(Gfx, L"G:\\Visual Studio Projects\\DirectX11LearnFromScrap\\��������\\globus_texture.jpeg", 3U));
        }
        AddBindable(std::make_unique<TransformBuffer>(Gfx, *this));
        AddBindable(std::make_unique<ColorBuffer>(Gfx, *this));
    }

    void MakeSkeleton()
    {
        SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
    }

    void MakeSolid(Graphics& Gfx)
    {
        ClearStaticBindables();
        struct Vertex
        {
            dx::XMFLOAT3 pos;
            dx::XMFLOAT3 n;
        };

        std::vector<D3D11_INPUT_ELEMENT_DESC> inputElementDesc =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
            { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
        };

        auto model = Sphere::MakeTesselated<Vertex>(40 , 40);

        std::unique_ptr<VertexShaderCommon> VS = std::make_unique<VertexShaderCommon>(Gfx, L"GeometryVS.cso");

        AddStaticBindable(std::make_unique<VertexBuffer>(Gfx, model.vertices));
        AddStaticBindable(std::make_unique<PixelShaderCommon>(Gfx, L"GeometryPS.cso"));
        AddStaticBindable(std::make_unique<InputLayout>(Gfx, inputElementDesc, VS.get()));
        AddStaticBindable(std::move(VS));
        AddStaticBindable(std::make_unique<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
        AddStaticBindable(std::make_unique<IndexBuffer>(Gfx, model.indices));
    }
    
    DirectX::XMMATRIX GetTransform() const noexcept override
    {
        return DOK_XMMatrixTranslation(world_position);
    }

    void SetScale(dx::XMFLOAT3 new_scale) noexcept
    {
        scale = new_scale;
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
    dx::XMFLOAT3 scale = { 1.f,1.f,1.f };
    dx::XMFLOAT3 world_position = { 0.f,0.f,0.f };
    dx::XMFLOAT4 color = { 1.f, 1.f, 1.f, 1.f };
};