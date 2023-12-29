#include "Mesh.h"
#include "hrException.h"
#include "DataBufferPS.h"

// ********** Mesh **********

Mesh::Mesh(Graphics& Gfx, aiMesh* p_Mesh, size_t v_id)
{
	// Get material index from material system in gfx (or create new and get index)
	std::shared_ptr<Material> pMat = Gfx.GetMaterialSystem().GetMaterialByIndex(v_id); assert(pMat);
	// Add material as bindable (binds material textures to pipeline when drawing mesh) if has some
	AddBindable(pMat);

	// Set mesh desc according to maps layout of material
	MapLayout v_mapLayout = pMat->GetMapLayout();
	v_MeshDesc.matId = (int)v_id;
	v_MeshDesc.useNormalMap		= v_mapLayout.hasNormalMap;
	v_MeshDesc.useDiffuseMap	= v_mapLayout.hasDiffuseMap;
	v_MeshDesc.useSpecularMap	= v_mapLayout.hasSpecularMap;
	v_MeshDesc.useHeightMap		= v_mapLayout.hasHeightMap;
	v_MeshDesc.useSpecularAlpha	= v_mapLayout.hasSpecularAlpha;
	AddBindable(PixelConstantBuffer<MeshDesc>::Resolve(Gfx, v_MeshDesc, 0U));

	if (pMat->HasAnyMaps())
		AddBindable(Sampler::Resolve(Gfx));

	std::shared_ptr<VertexShaderCommon> vertexShader = nullptr;
	DynamicVertex::VertexBuffer vertexBuffer;

	if (!pMat->HasAnyMaps())
	{
		vertexBuffer = DynamicVertex::VertexBuffer(std::move(
			DynamicVertex::VertexLayout{}
			.Append(DynamicVertex::VertexLayout::Position3D)
			.Append(DynamicVertex::VertexLayout::Normal)
		));
		for (unsigned int i = 0; i < p_Mesh->mNumVertices; i++)
		{
			vertexBuffer.EmplaceBack(
				*(reinterpret_cast<DirectX::XMFLOAT3*>(&p_Mesh->mVertices[i])),
				*(reinterpret_cast<DirectX::XMFLOAT3*>(&p_Mesh->mNormals[i]))
			);
		}

		// Add shaders
		vertexShader = VertexShaderCommon::Resolve(Gfx, L"shaders\\NotextureVS.cso");
		AddBindable(PixelShaderCommon::Resolve(Gfx, L"shaders\\NotexturePS.cso"));
	}
	else
	{
		vertexBuffer = DynamicVertex::VertexBuffer(std::move(
			DynamicVertex::VertexLayout{}
			.Append(DynamicVertex::VertexLayout::Position3D)
			.Append(DynamicVertex::VertexLayout::Normal)
			.Append(DynamicVertex::VertexLayout::Texture2D)
			.Append(DynamicVertex::VertexLayout::Tangent)
			.Append(DynamicVertex::VertexLayout::Bitangent)
		));

		for (unsigned int i = 0; i < p_Mesh->mNumVertices; i++)
		{
			vertexBuffer.EmplaceBack(
				*(reinterpret_cast<DirectX::XMFLOAT3*>(&p_Mesh->mVertices[i])),
				*(reinterpret_cast<DirectX::XMFLOAT3*>(&p_Mesh->mNormals[i])),
				*(reinterpret_cast<DirectX::XMFLOAT2*>(&p_Mesh->mTextureCoords[0][i])),
				*(reinterpret_cast<DirectX::XMFLOAT3*>(&p_Mesh->mTangents[i])),
				*(reinterpret_cast<DirectX::XMFLOAT3*>(&p_Mesh->mBitangents[i]))
			);
		}

		vertexShader = VertexShaderCommon::Resolve(Gfx, L"shaders\\NormalTextureVS.cso");
		AddBindable(PixelShaderCommon::Resolve(Gfx, L"shaders\\NormalTexturePS.cso"));
	}
	// Fill index buffer
	std::vector<unsigned short> indices;
	indices.reserve(static_cast<size_t>(p_Mesh->mNumFaces) * 3);

	// For each face retrieve it's indices 
	// Faces might be any form - not only triangles
	for (size_t i = 0; i < p_Mesh->mNumFaces; i++)
	{
		for (size_t k = 0; k < p_Mesh->mFaces[i].mNumIndices; k++) {
			indices.push_back(p_Mesh->mFaces[i].mIndices[k]);
		}
	}

	std::string meshTag = pMat->GetPath() + "#" + p_Mesh->mName.C_Str();
	AddBindable( IndexBuffer::Resolve(Gfx, meshTag, indices) );
	AddBindable( VertexBuffer::Resolve(Gfx, meshTag, vertexBuffer));
	AddBindable( InputLayout::Resolve(Gfx, vertexBuffer.GetLayout(), (IShader*)vertexShader.get()));
	AddBindable( std::move(vertexShader));
	AddBindable( std::make_shared<TransformBuffer>( Gfx, *this ) );
	AddBindable( Topology::Resolve(Gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST) );
}

void Mesh::Draw(Graphics& Gfx, DirectX::FXMMATRIX rusultTransform) const
{
	if (auto p_Buffer = QueryBindable<PixelConstantBuffer<MeshDesc>>())
		p_Buffer->Update(Gfx, v_MeshDesc); /* update buffer */

	dx::XMStoreFloat4x4(&this->rusultiveTransform, rusultTransform); // set resultive transform
	Drawable::Draw(Gfx);
}

std::unique_ptr<Mesh> Mesh::Load(Graphics& Gfx, aiMesh* p_Mesh, size_t v_id)
{
	return std::make_unique<Mesh>(Gfx, p_Mesh, v_id);
}

bool Mesh::ShowMeshGUI(Graphics& Gfx, std::string hash) noexcept
{
	auto makeHashed = [](std::string str, const std::string& hash) -> std::string
	{
		return str.append("##").append(hash);
	};
	bool changed = false;
	auto pMeshMaterial = Gfx.GetMaterialSystem().GetMaterialByIndex(v_MeshDesc.matId);
	MapLayout mapLayout = pMeshMaterial->GetMapLayout();

	if(mapLayout.hasDiffuseMap)
		changed |= ImGui::Checkbox(makeHashed("Use diffuse map", hash).c_str(), &v_MeshDesc.useDiffuseMap);
	if(!v_MeshDesc.useDiffuseMap)
		changed |= ImGui::ColorEdit4(makeHashed("Material albedo color", hash).c_str(), &v_MeshDesc.albedoColor.x);
	if(mapLayout.hasNormalMap)
		changed |= ImGui::Checkbox(makeHashed("Use normal map", hash).c_str(), &v_MeshDesc.useNormalMap);
	if(mapLayout.hasSpecularMap)
		changed |= ImGui::Checkbox(makeHashed("Use specular map", hash).c_str(), &v_MeshDesc.useSpecularMap);

	return changed;
}

int Mesh::GetMaterialIndex() const noexcept
{
	return v_MeshDesc.matId;
}

DirectX::XMMATRIX Mesh::GetTransform() const noexcept
{
	return DirectX::XMLoadFloat4x4( &rusultiveTransform );
}
