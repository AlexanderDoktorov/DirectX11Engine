#include "Mesh.h"
#include "Exceptions.h"

// ********** Mesh **********

Mesh::Mesh(Graphics& Gfx, std::vector<std::unique_ptr<IBindable>> pBindables, size_t matIndx) 
{
	AddBindable( std::make_unique<Topology>( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );


	for (size_t i = 0; i < pBindables.size(); i++)
	{
		AddBindable(std::move(pBindables[i]));
	}

	auto pMeshMaterial = Gfx.GetMaterialSystem().GetMaterialAt(matIndx);
	meshDesc.useDiffuseMap = pMeshMaterial->GetMapLayout().hasDiffuseMap;
	meshDesc.useNormalMap = pMeshMaterial->GetMapLayout().hasNormalMap;
	meshDesc.useSpecularMap = pMeshMaterial->GetMapLayout().hasSpecularMap;
	meshDesc.matId = (int)matIndx;

	AddBindable( std::make_unique<TransformBuffer>( Gfx,*this ) );
	AddBindable( std::make_unique<PixelConstantBuffer<MeshDesc>>(Gfx, meshDesc, 0U));
}

bool Mesh::ShowMeshGUI(Graphics& Gfx, std::string hash) noexcept
{
	typedef PixelConstantBuffer<MeshDesc> meshBuffer;
	Material* pMeshMaterial = Gfx.GetMaterialSystem().GetMaterialAt(meshDesc.matId);
	MapLayout mapLayout = pMeshMaterial->GetMapLayout();

	auto makeHased = [](std::string str, std::string hash) -> std::string
		{
			return str.append("##").append(hash);
		};

	bool changed = false;
	if(mapLayout.hasDiffuseMap)
		changed |= ImGui::Checkbox(makeHased("Use diffuse map", hash).c_str(), &meshDesc.useDiffuseMap);
	if(!meshDesc.useDiffuseMap)
		changed |= ImGui::ColorEdit3(makeHased("Material albedo color", hash).c_str(), &meshDesc.albedoColor.x);
	if(mapLayout.hasNormalMap)
		changed |= ImGui::Checkbox(makeHased("Use normal map", hash).c_str(), &meshDesc.useNormalMap);
	if(mapLayout.hasSpecularMap)
		changed |= ImGui::Checkbox(makeHased("Use specular map", hash).c_str(), &meshDesc.useSpecularMap);
	

	if (auto pMeshBuffer = QueryBindable<meshBuffer>(); changed && pMeshBuffer)
		pMeshBuffer->Update(Gfx, meshDesc);

	return changed;
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

