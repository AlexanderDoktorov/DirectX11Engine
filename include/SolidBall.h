#pragma once
#include "DOK_DX11.h"
#include "DrawableBase.h"
#include "HPipelineElements.h"
#include "Interfaces.h"
#include "Sphere.h"

class SolidBall : public DrawableBase<SolidBall>, public IColored, public IMovable, public IToString, public IScalable
{
public:
    SolidBall(Graphics& Gfx, dx::XMFLOAT4 solid_color = { 1.f,1.f,1.f,1.f })
    {
        SetColor(solid_color);
        if (!Initilized())
        {
            DynamicVertex::VertexLayout vertexLayout;
            vertexLayout.Append(DynamicVertex::VertexLayout::Position3D).Append(DynamicVertex::VertexLayout::Normal);
            auto model = Sphere::MakeTessaletedNormalized(vertexLayout, 41 , 20);

            std::unique_ptr<VertexShaderCommon> VS = std::make_unique<VertexShaderCommon>(Gfx, L"shaders\\GeometryVS.cso");
            AddStaticBindable(std::make_unique<VertexBuffer>(Gfx, model.vertices));
            AddStaticBindable(std::make_unique<PixelShaderCommon>(Gfx, L"shaders\\GeometryPS.cso"));
            AddStaticBindable(std::make_unique<InputLayout>(Gfx, vertexLayout, VS.get()));
            AddStaticBindable(std::move(VS));
            AddStaticBindable(std::make_unique<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
            AddStaticBindable(std::make_unique<IndexBuffer>(Gfx, model.indices));
            AddStaticBindable(std::make_unique<WICTexture>(Gfx, L"G:\\Visual Studio Projects\\DirectX11LearnFromScrap\\Текстуры\\globus_texture.jpeg", 3U));
        }
        AddBindable(std::make_unique<TransformBuffer>(Gfx, *this));
        AddBindable(std::make_unique<ColorBuffer>(Gfx, *this));
    }

    void MakeSkeleton()
    {
        if (auto tp = QueryBindable<Topology>())
        {
            tp->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
        }
    }
    
    virtual DirectX::XMMATRIX GetTransform() const noexcept override
    {
        return DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw,  roll) * DirectX::XMMatrixScaling(scale.x, scale.y, scale.z) * DirectX::XMMatrixTranslation(world_position.x, world_position.y, world_position.z);
    }

    void SetScale(dx::XMFLOAT3 new_scale) noexcept
    {
        scale = new_scale;
    }

    // IMovable
    void SetPosition(float _x, float _y, float _z) override;
    DirectX::XMFLOAT3 GetPosition() const noexcept override;

    // IToString
    const char* ToString() const noexcept override;

    // IColored
    DirectX::XMFLOAT4 GetColor() const noexcept override;
    void SetColor(dx::XMFLOAT4 new_color) noexcept override;

    // IScalable
    virtual void SetScaleX(const float& scale_x_new) override;
    virtual void SetScaleY(const float& scale_y_new) override;
    virtual void SetScaleZ(const float& scale_z_new) override;
    virtual dx::XMFLOAT3 GetScale() const noexcept	 override;

protected:
    dx::XMFLOAT3 scale = { 1.f, 1.f, 1.f };
    dx::XMFLOAT3 world_position = { 0.f, 0.f, 0.f };
    dx::XMFLOAT4 color = { 1.f, 1.f, 1.f, 1.f };
    float roll = 0.f;
    float pitch = 0.f;
    float yaw = 0.f;
};