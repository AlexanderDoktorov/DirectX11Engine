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

void Mesh::ShowMeshControls(Graphics& Gfx) noexcept
{
	typedef PixelConstantBuffer<MeshDesc> meshBuffer;
	static constexpr ImVec4 redColor	  = ImVec4(1.f, 0.f, 0.f, 1.f);
	static constexpr ImVec4 yellowColor   = ImVec4(1.f, 1.f, 0.f, 1.f);
	auto makeStr = [&](const char* text) -> std::string
	{
		return text + std::string("##") + std::to_string(meshDesc.matId);
	};

	ImGui::Text("Mesh with material id = %d", meshDesc.matId);
		
	bool changed = false;
	ImGui::TextColored(meshDesc.useSpecularMap ? yellowColor : redColor , "Use specular map: %s", meshDesc.useSpecularMap ? "True" : "False");
	ImGui::TextColored(meshDesc.useNormalMap ? yellowColor : redColor , "Use normal map: %s", meshDesc.useNormalMap ? "True" : "False");
	ImGui::TextColored(meshDesc.useDiffuseMap ? yellowColor : redColor , "Use diffuse map: %s", meshDesc.useDiffuseMap ? "True" : "False");
	if(!meshDesc.useDiffuseMap)
		changed |= ImGui::ColorEdit3(makeStr("Albedo color").c_str(), &meshDesc.albedoColor.x);

	if (auto pMeshBuffer = QueryBindable<meshBuffer>(); changed && pMeshBuffer)
		pMeshBuffer->Update(Gfx, meshDesc);
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

