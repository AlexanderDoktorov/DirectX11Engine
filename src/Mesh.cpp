#include "Mesh.h"
#include "Exceptions.h"
#include "DataBufferPS.h"

// ********** Mesh **********

Mesh::Mesh(Graphics& Gfx, std::vector<std::shared_ptr<IBindable>> pBindables, size_t mId)
{
	const auto pMat = Gfx.GetMaterialSystem().GetMaterialAt(mId);
	if (pMat->HasAnyMaps())
	{
		MapLayout ml = pMat->GetMapLayout();
		MeshDesc temp{};
		temp.useDiffuseMap = ml.hasDiffuseMap;
		temp.useNormalMap = ml.hasNormalMap;
		temp.useSpecularMap = ml.hasSpecularMap;
		temp.matId = (int)mId;
		meshDesc = std::move(temp);
		AddBindable( std::make_shared<DataBufferPS<MeshDesc, 0U>>(Gfx, std::get<MeshDesc>(meshDesc)));
	}
	else
	{
		MeshDescNotex temp{};
		temp.matId = (int)mId;
		meshDesc = std::move(temp);
		AddBindable( std::make_shared<DataBufferPS<MeshDescNotex, 0U>>(Gfx, std::get<MeshDescNotex>(meshDesc)));
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
	if (auto pDesc = std::get_if<MeshDesc>(&meshDesc))
	{
		auto pMeshMaterial = Gfx.GetMaterialSystem().GetMaterialAt(pDesc->matId);
		MapLayout mapLayout = pMeshMaterial->GetMapLayout();

		if(mapLayout.hasDiffuseMap)
			changed |= ImGui::Checkbox(makeHashed("Use diffuse map", hash).c_str(), &pDesc->useDiffuseMap);
		if(!pDesc->useDiffuseMap)
			changed |= ImGui::ColorEdit4(makeHashed("Material albedo color", hash).c_str(), &pDesc->albedoColor.x);
		if(mapLayout.hasNormalMap)
			changed |= ImGui::Checkbox(makeHashed("Use normal map", hash).c_str(), &pDesc->useNormalMap);
		if(mapLayout.hasSpecularMap)
			changed |= ImGui::Checkbox(makeHashed("Use specular map", hash).c_str(), &pDesc->useSpecularMap);

		if(auto pMeshBuffer = QueryBindable<meshBufferTextured>(); changed && pMeshBuffer)
			pMeshBuffer->Update(Gfx, *pDesc);
	}
	if (auto pNoTexDesc = std::get_if<MeshDescNotex>(&meshDesc))
	{
		changed |= ImGui::ColorEdit4(makeHashed("Material albedo color", hash).c_str(), &pNoTexDesc->albedoColor.x);
		if (auto pMeshBufferNoTex = QueryBindable<meshBufferNoTex>(); changed && pMeshBufferNoTex)
			pMeshBufferNoTex->Update(Gfx, *pNoTexDesc);
	}
	return changed;
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

