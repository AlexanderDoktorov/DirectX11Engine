#include "Character.h"
#include "Mesh.h"

Character::Character(Graphics& Gfx)
{
	if (!DrawableBase::Initilized())
	{
		DynamicVertex::VertexLayout vl({DynamicVertex::VertexLayout::Position3D, DynamicVertex::VertexLayout::Normal});

		// Model mesh{ "Models\\CharacterMesh.obj", vl, aiPostProcessSteps::aiProcess_JoinIdenticalVertices | aiPostProcessSteps::aiProcess_Triangulate };

		auto VS = std::make_unique<VertexShaderCommon>(Gfx, L"shaders\\GeometryVS.cso");
		// AddStaticBindable(std::make_unique<VertexBuffer>(Gfx, mesh.pItl->vertices));
		// AddStaticBindable(std::make_unique<IndexBuffer>(Gfx, mesh.pItl->indices));
		AddStaticBindable(std::make_unique<InputLayout>(Gfx, vl.GetD3DLayout(), (IShader*)VS.get()));
		AddStaticBindable(std::move(VS));
		AddStaticBindable(std::make_unique<PixelShaderCommon>(Gfx, L"shaders\\GeometryPS.cso"));
		AddStaticBindable(std::make_unique<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}
	AddBindable(std::make_unique<TransformBuffer>(Gfx, *this));
}

const char* Character::ToString() const noexcept
{
	return "Character";
}

DirectX::XMMATRIX Character::GetTransform() const noexcept
{
	return dx::XMMatrixTranslation(pos.x, pos.y, pos.z) * dx::XMMatrixScaling(scale.x, scale.y, scale.z);
}

void Character::SetScaleX(const float& scale_x_new)
{
	scale.x = scale_x_new;
}

void Character::SetScaleY(const float& scale_y_new)
{
	scale.y = scale_y_new;
}

void Character::SetScaleZ(const float& scale_z_new)
{
	scale.z = scale_z_new;
}

dx::XMFLOAT3 Character::GetScale() const noexcept
{
	return scale;
}
