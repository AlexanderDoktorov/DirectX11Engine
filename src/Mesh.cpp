#include "Mesh.h"
#include "Exceptions.h"
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
		AddBindable( std::make_shared<PixelConstantBuffer<MeshDesc>>(Gfx, *pMeshDescTextured, 0U));
	}
	else
	{
		pMeshDescUntextured = std::make_unique<MeshDescNotex>();
		pMeshDescUntextured->matId = (uint32_t)mId;
		AddBindable( std::make_shared<PixelConstantBuffer<MeshDescNotex>>(Gfx, *pMeshDescUntextured, 0U));
	}

	AddBindable( Topology::Resolve(Gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST) );

	for (size_t i = 0; i < pBindables.size(); i++)
	{
		AddBindable(std::move(pBindables[i]));
	}

	AddBindable( std::make_shared<TransformBuffer>( Gfx,*this ) );
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

		if(auto pMeshBuffer = QueryBindable<PixelConstantBuffer<MeshDesc>>(); changed && pMeshBuffer)
			pMeshBuffer->Update(Gfx, *pMeshDescTextured);
	}
	if (pMeshDescUntextured)
	{
		changed |= ImGui::ColorEdit4(makeHashed("Material albedo color", hash).c_str(), &pMeshDescUntextured->albedoColor.x);
		if (auto pMeshBufferNoTex = QueryBindable<PixelConstantBuffer<MeshDescNotex>>(); changed && pMeshBufferNoTex)
			pMeshBufferNoTex->Update(Gfx, *pMeshDescUntextured);
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
	return 
		DirectX::XMMatrixRotationRollPitchYaw(worldRotation.y, worldRotation.z, worldRotation.x) *
		DirectX::XMMatrixTranslation(worldTranslation.x, worldTranslation.y, worldTranslation.z) * 
		DirectX::XMMatrixScaling(scale,scale,scale);
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

Mesh& Mesh::Scale(float scaleXYZ) noexcept
{
	scale = scaleXYZ;
	return *this;
}

