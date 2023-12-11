#include "Mesh.h"
#include "Exceptions.h"

// ********** Mesh **********

Mesh::Mesh(Graphics& Gfx, std::vector<std::unique_ptr<IBindable>> pBindables, size_t matIndx)
{
	meshDesc.matId = static_cast<int>(matIndx);

	AddBindable( std::make_unique<Topology>( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );

	for (size_t i = 0; i < pBindables.size(); i++)
	{
		AddBindable(std::move(pBindables[i]));
	}

	AddBindable( std::make_unique<TransformBuffer>( Gfx,*this ) );
}

int Mesh::GetMaterialIndex() const noexcept
{
	return meshDesc.matId;
}

DirectX::XMMATRIX Mesh::GetTransform() const noexcept
{
	return DirectX::XMMatrixTranslation(worldTranslation.x, worldTranslation.y, worldTranslation.z);
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

