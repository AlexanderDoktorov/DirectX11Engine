#include "Mesh.h"
#include "hrException.h"
#include "DataBufferPS.h"
#include "VertexBuffer.h"
#include "InputLayout.h"
#include "Topology.h"
#include "TransformBuffer.h"
#include "PixelShaderCommon.h"
#include "VertexShaderCommon.h"
#include "Sampler.h"

// ********** Mesh **********

Mesh::Mesh(Graphics& Gfx, const aiMesh* p_Mesh, const aiMaterial* p_aiMaterial, std::string directory) 
	:
	p_MeshMaterial(std::make_shared<Material>(Gfx, p_aiMaterial, directory))
{
	AddBindable(p_MeshMaterial);

	if (p_MeshMaterial->HasAnyMaps())
		AddBindable(Sampler::Resolve(Gfx));

	std::shared_ptr<VertexShaderCommon> vertexShader = nullptr;
	DynamicVertex::VertexBuffer vertexBuffer;

	MAP_FLAG mapFlags = p_MeshMaterial->GetMapsFlags();

	if (!p_MeshMaterial->HasAnyMaps())
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
		vertexShader = VertexShaderCommon::Resolve(Gfx, L"shaders\\NotexVS.cso");
		AddDefferedBindable(PixelShaderCommon::Resolve(Gfx, L"shaders\\NotexturePS_deffered.cso"));
		AddForwardBindable(PixelShaderCommon::Resolve(Gfx, L"shaders\\NotexturePS_forward.cso"));
	}
	else if(mapFlags & (MAP_FLAG_DIFF | MAP_FLAG_NORMAL | MAP_FLAG_SPEC))
	{ // DiffNormalSpec case
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

		vertexShader = VertexShaderCommon::Resolve(Gfx, L"shaders\\NormalVS.cso");
		AddDefferedBindable(PixelShaderCommon::Resolve(Gfx, L"shaders\\DiffNormalSpecularPS_deffered.cso"));
		AddForwardBindable(PixelShaderCommon::Resolve(Gfx, L"shaders\\DiffNormalSpecularPS_forward.cso"));
	}
	else if (mapFlags & (MAP_FLAG_DIFF | MAP_FLAG_NORMAL))
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

		vertexShader = VertexShaderCommon::Resolve(Gfx, L"shaders\\NormalVS.cso");
		AddDefferedBindable(PixelShaderCommon::Resolve(Gfx, L"shaders\\DiffNormalPS_deffered.cso"));
		AddForwardBindable(PixelShaderCommon::Resolve(Gfx, L"shaders\\DiffNormalPS_forward.cso"));
	}
	else if (mapFlags & (MAP_FLAG_DIFF | MAP_FLAG_SPEC))
	{
		vertexBuffer = DynamicVertex::VertexBuffer(std::move(
			DynamicVertex::VertexLayout{}
			.Append(DynamicVertex::VertexLayout::Position3D)
			.Append(DynamicVertex::VertexLayout::Normal)
			.Append(DynamicVertex::VertexLayout::Texture2D)
		));

		for (unsigned int i = 0; i < p_Mesh->mNumVertices; i++)
		{
			vertexBuffer.EmplaceBack(
				*(reinterpret_cast<DirectX::XMFLOAT3*>(&p_Mesh->mVertices[i])),
				*(reinterpret_cast<DirectX::XMFLOAT3*>(&p_Mesh->mNormals[i])),
				*(reinterpret_cast<DirectX::XMFLOAT2*>(&p_Mesh->mTextureCoords[0][i]))
			);
		}

		vertexShader = VertexShaderCommon::Resolve(Gfx, L"shaders\\AbnormalVS.cso");
		AddDefferedBindable(PixelShaderCommon::Resolve(Gfx, L"shaders\\DiffSpecularPS_deffered.cso"));
		AddForwardBindable(PixelShaderCommon::Resolve(Gfx, L"shaders\\DiffSpecularPS_forward.cso"));
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

	std::string meshTag = p_MeshMaterial->GetPath() + "#" + p_Mesh->mName.C_Str();
	AddBindable( IndexBuffer::Resolve(Gfx, meshTag, indices) );
	AddBindable( VertexBuffer::Resolve(Gfx, meshTag, vertexBuffer));
	AddBindable( InputLayout::Resolve(Gfx, vertexBuffer.GetLayout(), (IShader*)vertexShader.get()));
	AddBindable( std::move(vertexShader));
	AddBindable( std::make_shared<TransformBuffer>( Gfx, *this ) );
	AddBindable( Topology::Resolve(Gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST) );
}

void Mesh::Draw(Graphics& Gfx, DirectX::FXMMATRIX rusultTransform) const
{
	dx::XMStoreFloat4x4(&this->rusultiveTransform, rusultTransform); // set resultive transform
	Drawable::Draw(Gfx);
}

bool Mesh::ShowMeshGUI(Graphics& Gfx, std::string hash) noexcept
{
	return p_MeshMaterial->ShowMaterialGUI();
}

DirectX::XMMATRIX Mesh::GetTransform() const noexcept
{
	return DirectX::XMLoadFloat4x4( &rusultiveTransform );
}
