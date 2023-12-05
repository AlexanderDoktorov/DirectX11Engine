#include "Model.h"

// ********** Model **********

Model::Model(Graphics& Gfx, const std::string& fileName, unsigned int aippFlags)
{
	Assimp::Importer imp;
	const aiScene* pScene = imp.ReadFile(fileName.c_str(), aippFlags);

	if(!pScene || pScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !pScene->mRootNode) // if is Not Zero
	{
		OutputDebugStringA("ERROR ASSIMP: ");
		OutputDebugStringA(imp.GetErrorString());
		return;
	}

	directory = fileName.substr(0, fileName.find_last_of('\\'));


	// Fill materialPtrs array with all scene materials and push them to structured buffer
	materialsPtrs.reserve(pScene->mNumMaterials);
	for( size_t i = 0; i < pScene->mNumMaterials; i++ )
	{
		auto pMaterial = std::make_shared<Material>(Gfx, pScene->mMaterials[i], directory);
		Gfx.UpdateMaterialAt(pMaterial->GetDesc(), pMaterial->GetIndex());
		materialsPtrs.push_back( std::move(pMaterial) );
	}

	// Fill meshesPtrs array with all scene meshes
	meshesPtrs.reserve(pScene->mNumMeshes);
	for( size_t i = 0; i < pScene->mNumMeshes; i++ )
	{
		meshesPtrs.push_back( ProccesMesh(Gfx, pScene->mMeshes[i], pScene) );
	}

	int StartId = 0;
	pRootNode = ProcessNode(Gfx, StartId, pScene->mRootNode);
}

void Model::ShowControlWindow(Graphics& Gfx) noexcept
{
	// Materials options
	for (size_t i = 0; i < materialsPtrs.size(); i++)
		materialsPtrs[i]->ShowControlWindow(Gfx);
	// Meshes options
	for (size_t i = 0; i < meshesPtrs.size(); i++)
		meshesPtrs[i]->ShowControlWindow(Gfx);
}

void Model::Draw(Graphics& Gfx)
{
	for (size_t i = 0; i < meshesPtrs.size(); i++)
	{
		meshesPtrs[i]->Draw(Gfx);
	}
}

std::unique_ptr<Node> Model::ProcessNode(Graphics& Gfx, int& startID, aiNode* pRootNode)
{
	// fill nodeMeshes pointers for building a node
	std::vector<Mesh*> nodeMeshes;
	nodeMeshes.reserve(pRootNode->mNumMeshes);
	for(unsigned int i = 0; i < pRootNode->mNumMeshes; i++)
	{
		const unsigned int meshIndx = pRootNode->mMeshes[i];
		nodeMeshes.push_back(meshesPtrs.at(meshIndx).get());
	}

	// construct root with meshes
	std::unique_ptr<Node> pMyNode = std::make_unique<Node>(startID++, pRootNode->mName.C_Str(), nodeMeshes);
	
	pMyNode->Reserve(pRootNode->mNumChildren);
	// add children for node by building them the same way
	for(unsigned int i = 0; i < pRootNode->mNumChildren; i++)
	{
		pMyNode->AddChild(ProcessNode(Gfx, startID, pRootNode->mChildren[i]));
	}

	return pMyNode;
}

std::unique_ptr<Mesh> Model::ProccesMesh(Graphics& Gfx, aiMesh* pMesh, const aiScene* pScene)
{
	std::vector<std::unique_ptr<IBindable>> bindablePtrs;

	auto& pMat = materialsPtrs.at(pMesh->mMaterialIndex);

	// Load materials into arrays for each mesh
	std::unique_ptr<TextureArrayPS> diffuseMapsPS	= std::make_unique<TextureArrayPS>(Gfx,  SLOT_TEXTURE_ARRAY_DIFFUSE);
	std::unique_ptr<TextureArrayPS> normalMapsPS	= std::make_unique<TextureArrayPS>(Gfx,  SLOT_TEXTURE_ARRAY_NORMALMAP);
	std::unique_ptr<TextureArrayPS> specularMapsPS	= std::make_unique<TextureArrayPS>(Gfx,  SLOT_TEXTURE_ARRAY_SPECULAR);
	std::unique_ptr<TextureArrayPS> heightMapsPS	= std::make_unique<TextureArrayPS>(Gfx,  SLOT_TEXTURE_ARRAY_HEIGHT);

	for (const auto& text : pMat->GetTextures())
	{
		const char* textureFilePath = text->GetFilePath_C_str();
		switch (text->GetTextureAiType())
		{
		case aiTextureType_DIFFUSE:
			diffuseMapsPS->PushBack(Gfx, textureFilePath);
			break;
		case aiTextureType_SPECULAR:
			specularMapsPS->PushBack(Gfx, textureFilePath);
			break;
		case aiTextureType_HEIGHT:
			heightMapsPS->PushBack(Gfx, textureFilePath);
			break;
		case aiTextureType_NORMALS:
			normalMapsPS->PushBack(Gfx, textureFilePath, DirectX::WIC_LOADER_IGNORE_SRGB);
			break;
		default:
			break;
		}
	}

	const bool HasDiffuseMaps		= !diffuseMapsPS->Empty();
	const bool HasNormalMaps		= !normalMapsPS->Empty();
	const bool HasSpecularMaps		= !specularMapsPS->Empty();
	const bool HasHeightMaps		= !heightMapsPS->Empty();
	const bool HasAnyMaps			= HasDiffuseMaps || HasNormalMaps || HasSpecularMaps || HasHeightMaps;

	if (HasDiffuseMaps)
		bindablePtrs.push_back(std::move(diffuseMapsPS));
	if (HasNormalMaps)
		bindablePtrs.push_back(std::move(normalMapsPS));
	if (HasSpecularMaps)
		bindablePtrs.push_back(std::move(specularMapsPS));
	if (HasHeightMaps)
		bindablePtrs.push_back(std::move(heightMapsPS));
	if (HasAnyMaps)
		bindablePtrs.push_back(std::make_unique<Sampler>(Gfx));

	if (pMat->HasDiffuseMaps() && HasNormalMaps && HasSpecularMaps)
	{
		// TO DO
	}
	else if (HasDiffuseMaps && HasNormalMaps)
	{
		DynamicVertex::VertexBuffer vb(std::move(
			DynamicVertex::VertexLayout{}
			.Append(DynamicVertex::VertexLayout::Position3D)
			.Append(DynamicVertex::VertexLayout::Normal)
			.Append(DynamicVertex::VertexLayout::Texture2D)
			.Append(DynamicVertex::VertexLayout::Tangent)
			.Append(DynamicVertex::VertexLayout::Bitangent)
		));

		for (unsigned int i = 0; i < pMesh->mNumVertices; i++)
		{
			vb.EmplaceBack(
				*(reinterpret_cast<DirectX::XMFLOAT3*>(&pMesh->mVertices[i])),
				*(reinterpret_cast<DirectX::XMFLOAT3*>(&pMesh->mNormals[i])),
				*(reinterpret_cast<DirectX::XMFLOAT2*>(&pMesh->mTextureCoords[0][i])),
				*(reinterpret_cast<DirectX::XMFLOAT3*>(&pMesh->mTangents[i])),
				*(reinterpret_cast<DirectX::XMFLOAT3*>(&pMesh->mBitangents[i]))
			);
		}

		auto VS = std::make_unique<VertexShaderCommon>(Gfx, L"shaders\\NormalTextureVS.cso");
		bindablePtrs.push_back(std::make_unique<VertexBuffer>(Gfx, vb));
		bindablePtrs.push_back(std::make_unique<InputLayout>(Gfx, vb.GetLayout().GetD3DLayout(), (IShader*)VS.get()));
		bindablePtrs.push_back(std::move(VS));
		bindablePtrs.push_back(std::make_unique<PixelShaderCommon>(Gfx, L"shaders\\NormalTexturePS.cso"));
	}

	// Fill index buffer
	std::vector<unsigned short> indices;
	indices.reserve(pMesh->mNumFaces * 3);

	// For each face retrieve it's indices 
	// Faces might be any form - not only triangles
	for (size_t i = 0; i < pMesh->mNumFaces; i++)
	{
		for (size_t k = 0; k < pMesh->mFaces[i].mNumIndices; k++)
			indices.push_back(pMesh->mFaces[i].mIndices[k]);
	}
	bindablePtrs.push_back( std::make_unique<IndexBuffer>(Gfx, indices));


	return std::make_unique<Mesh>(Gfx, pMat.get(), std::move(bindablePtrs));
}