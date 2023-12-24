#include "Mesh.h"
#include "hrException.h"
#include "DataBufferPS.h"

// ********** Mesh **********

Mesh::Mesh(Graphics& Gfx, std::vector<std::shared_ptr<IBindable>> pBindables, size_t mId)
{
	const auto pMat = Gfx.GetMaterialSystem().GetMaterialByIndex(mId);
	if (pMat->HasAnyMaps())
	{
		MapLayout ml = pMat->GetMapLayout();
		pMeshDescTextured = std::make_unique<MeshDesc>();
		pMeshDescTextured->useDiffuseMap = ml.hasDiffuseMap;
		pMeshDescTextured->useNormalMap = ml.hasNormalMap;
		pMeshDescTextured->useSpecularMap = ml.hasSpecularMap;
		pMeshDescTextured->useSpecularMapColored = ml.hasSpecularMapColored;
		pMeshDescTextured->matId = (uint32_t)mId;
		AddBindable( PixelConstantBuffer<MeshDesc>::Resolve(Gfx, *pMeshDescTextured, 0U));
	}
	else
	{
		pMeshDescUntextured = std::make_unique<MeshDescNotex>();
		pMeshDescUntextured->matId = (uint32_t)mId;
		AddBindable( PixelConstantBuffer<MeshDescNotex>::Resolve(Gfx, *pMeshDescUntextured, 0U));
	}

	AddBindable( Topology::Resolve(Gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST) );

	for (size_t i = 0; i < pBindables.size(); i++)
	{
		AddBindable(std::move(pBindables[i]));
	}

	AddBindable( std::make_shared<TransformBuffer>( Gfx,*this ) );
}

void Mesh::Draw(Graphics& Gfx, DirectX::FXMMATRIX rusultTransform) const
{
	// Update constant buffer with current mesh data
	// If it is textured
	if (pMeshDescTextured)
		if (auto pBuffer = QueryBindable<PixelConstantBuffer<MeshDesc>>())
			pBuffer->Update(Gfx, *pMeshDescTextured);// If it is textured
	// If it is not textured
	if (pMeshDescUntextured)
		if (auto pBuffer = QueryBindable<PixelConstantBuffer<MeshDescNotex>>())
			pBuffer->Update(Gfx, *pMeshDescUntextured);
	// And draw
	dx::XMStoreFloat4x4(&this->rusultiveTransform, rusultTransform); // set resultive transform
	Drawable::Draw(Gfx);
}

bool Mesh::ShowMeshGUI(Graphics& Gfx, std::string hash) noexcept
{
	auto makeHashed = [](std::string str, const std::string& hash) -> std::string
		{
			return str.append("##").append(hash);
		};
	bool changed = false;
	if (pMeshDescTextured)
	{
		auto pMeshMaterial = Gfx.GetMaterialSystem().GetMaterialByIndex(pMeshDescTextured->matId);
		MapLayout mapLayout = pMeshMaterial->GetMapLayout();

		if(mapLayout.hasDiffuseMap)
			changed |= ImGui::Checkbox(makeHashed("Use diffuse map", hash).c_str(), &pMeshDescTextured->useDiffuseMap);
		if(!pMeshDescTextured->useDiffuseMap)
			changed |= ImGui::ColorEdit4(makeHashed("Material albedo color", hash).c_str(), &pMeshDescTextured->albedoColor.x);
		if(mapLayout.hasNormalMap)
			changed |= ImGui::Checkbox(makeHashed("Use normal map", hash).c_str(), &pMeshDescTextured->useNormalMap);
		if(mapLayout.hasSpecularMap)
			changed |= ImGui::Checkbox(makeHashed("Use specular map", hash).c_str(), &pMeshDescTextured->useSpecularMap);
	}
	if (pMeshDescUntextured)
	{
		changed |= ImGui::ColorEdit4(makeHashed("Material albedo color", hash).c_str(), &pMeshDescUntextured->albedoColor.x);
	}
	return changed;
}

int Mesh::GetMaterialIndex() const noexcept
{
	if (pMeshDescTextured)
		return pMeshDescTextured->matId;
	else
		return pMeshDescUntextured->matId;
}

DirectX::XMMATRIX Mesh::GetTransform() const noexcept
{
	return DirectX::XMLoadFloat4x4( &rusultiveTransform );
}
