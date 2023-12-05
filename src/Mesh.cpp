#include "Mesh.h"
#include "Exceptions.h"

// ********** Mesh **********

Mesh::Mesh(Graphics& Gfx, const Material* pMeshMaterial, std::vector<std::unique_ptr<IBindable>> pBindables) 
{
	AddBindable( std::make_unique<Topology>( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );


	for (size_t i = 0; i < pBindables.size(); i++)
	{
		AddBindable(std::move(pBindables[i]));
	}

	AddBindable( std::make_unique<TransformBuffer>( Gfx,*this ) );

	meshDesc.matId			= pMeshMaterial->GetIndex();
	meshDesc.useDiffuseMap	= pMeshMaterial->HasDiffuseMaps();
	meshDesc.useNormalMap	= pMeshMaterial->HasNormalMaps();
	meshDesc.useSpecularMap = pMeshMaterial->HasSpecularMaps();
	AddBindable (std::make_unique<PixelConstantBuffer<MeshDesc>>(Gfx, meshDesc, 0U));
}

void Mesh::ShowControlWindow(Graphics& Gfx) noexcept
{
	typedef PixelConstantBuffer<MeshDesc> meshBuffer;
	if (auto pMeshBuffer = QueryBindable<meshBuffer>())
	{
		if (ImGui::Begin( ("Mesh control window with material id = " + std::to_string(meshDesc.matId)).c_str() ))
		{
			bool changed = false;
			changed |= ImGui::Checkbox("Use normal map", &meshDesc.useNormalMap);
			changed |= ImGui::Checkbox("Use diffuse map", &meshDesc.useDiffuseMap);
			if(!meshDesc.useDiffuseMap)
				changed |= ImGui::ColorEdit3("Albedo color", &meshDesc.albedoColor.x);
			changed |= ImGui::Checkbox("Use specular map", &meshDesc.useSpecularMap);

			if (changed)
			{
				pMeshBuffer->Update(Gfx, meshDesc);
				auto newDesc = Gfx.GetMaterialAt(meshDesc.matId);
				newDesc.hasDiffuseMap = meshDesc.useDiffuseMap;
				newDesc.hasNormalMap = meshDesc.useNormalMap;
				newDesc.hasSpecularMap = meshDesc.useSpecularMap;
				Gfx.UpdateMaterialAt(newDesc, meshDesc.matId);
			}
		}
		ImGui::End();
	}
}

int Mesh::GetMaterialIndex() const noexcept
{
	return meshDesc.matId;
}

