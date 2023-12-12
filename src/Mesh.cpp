#include "Mesh.h"
#include "Exceptions.h"

// ********** Mesh **********

Mesh::Mesh(Graphics& Gfx, std::vector<std::unique_ptr<IBindable>> pBindables, size_t matIndx)
{
	const Material* pMat = Gfx.GetMaterialSystem().GetMaterialAt(matIndx);
	if (pMat->HasAnyMaps())
	{
		MapLayout ml = pMat->GetMapLayout();
		MeshDesc temp{};
		temp.useDiffuseMap = ml.hasDiffuseMap;
		temp.useNormalMap = ml.hasNormalMap;
		temp.useSpecularMap = ml.hasSpecularMap;
		temp.matId = (int)matIndx;
		meshDesc = std::move(temp);
		AddBindable(std::make_unique<PixelConstantBuffer<Mesh::MeshDesc>>(Gfx, std::get<MeshDesc>(meshDesc), 0U));
	}
	else
	{
		MeshDescNotex temp{};
		temp.matId = (int)matIndx;
		meshDesc = std::move(temp);
		AddBindable(std::make_unique<PixelConstantBuffer<Mesh::MeshDescNotex>>(Gfx, std::get<MeshDescNotex>(meshDesc), 0U));
	}

	AddBindable( std::make_unique<Topology>( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );

	for (size_t i = 0; i < pBindables.size(); i++)
	{
		AddBindable(std::move(pBindables[i]));
	}

	AddBindable( std::make_unique<TransformBuffer>( Gfx,*this ) );
}

int Mesh::GetMaterialIndex() const noexcept
{
	if (auto texturedDesc = std::get_if<MeshDesc>(&meshDesc))
		return texturedDesc->matId;
	else
		return std::get<MeshDescNotex>(meshDesc).matId;
}

DirectX::XMMATRIX Mesh::GetTransform() const noexcept
{
	return DirectX::XMMatrixTranslation(worldTranslation.x, worldTranslation.y, worldTranslation.z) * DirectX::XMMatrixRotationRollPitchYaw(worldRotation.y, worldRotation.z, worldRotation.x);
}

Mesh& Mesh::Translate(float dx, float dy, float dz) noexcept
{
	worldTranslation.x += dx;
	worldTranslation.y += dy;
	worldTranslation.z += dz;
	return *this;
}

Mesh& Mesh::SetPosition(float x, float y, float z) noexcept
{
	worldTranslation.x = x;
	worldTranslation.y = y;
	worldTranslation.z = z;
	return *this;
}

Mesh& Mesh::SetRotatin(float roll, float pitch, float yaw) noexcept
{
	worldRotation.x = roll;
	worldRotation.y = pitch;
	worldRotation.z = yaw;
	return *this;
}

