#include "Character.h"

Character::Character(Graphics& Gfx)
{
	if (!DrawableBase::Initilized())
	{
		DynamicVertex::VertexLayout vl({DynamicVertex::VertexLayout::Position3D, DynamicVertex::VertexLayout::Normal});

		Mesh mesh{ "Models\\CharacterMesh.obj", vl, aiPostProcessSteps::aiProcess_JoinIdenticalVertices | aiPostProcessSteps::aiProcess_Triangulate };
		mesh.pItl->SetNormalsSmooth();

		auto VS = std::make_unique<VertexShaderCommon>(Gfx, L"GeometryVS.cso");
		AddStaticBindable(std::make_unique<VertexBuffer>(Gfx, mesh.pItl->vertices));
		AddStaticBindable(std::make_unique<IndexBuffer>(Gfx, mesh.pItl->indices));
		AddStaticBindable(std::make_unique<InputLayout>(Gfx, vl.GetD3DLayout(), (IShader*)VS.get()));
		AddStaticBindable(std::move(VS));
		AddStaticBindable(std::make_unique<PixelShaderCommon>(Gfx, L"GeometryPS.cso"));
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

dx::XMFLOAT3& Character::GetScaleRef() noexcept
{
	return scale;
}