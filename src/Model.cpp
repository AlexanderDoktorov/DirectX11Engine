#include "Model.h"

// ********** Model **********

Model::Model(Graphics& Gfx, const std::string& fileName, unsigned int aippFlags)
	:
	modelNum(GetCount())
{
	Load(Gfx, fileName, aippFlags);
}

void Model::Load(Graphics& Gfx, const std::string& fileName, unsigned int aippFlags) noexcept
{
	ClearData();

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
	materialsIndices.reserve(pScene->mNumMaterials);
	for( size_t i = 0; i < pScene->mNumMaterials; i++ )
	{
		auto mat = Material(Gfx, pScene->mMaterials[i], directory);
		materialsIndices.push_back(Gfx.GetMaterialSystem().GetMaterialIndex(mat));
	}

	// Fill meshesPtrs array with all scene meshes
	meshesPtrs.reserve(pScene->mNumMeshes);
	for( size_t i = 0; i < pScene->mNumMeshes; i++ )
	{
		meshesPtrs.push_back( ProccesMesh(Gfx, pScene->mMeshes[i], materialsIndices[pScene->mMeshes[i]->mMaterialIndex] ) );
	}

	int StartId = 0;
	pRootNode = ProcessNode(Gfx, StartId, pScene->mRootNode);
}

void Model::ClearData() noexcept
{
	meshesPtrs.clear();
	materialsIndices.clear();
	if(pRootNode)
		pRootNode->Clear();
	directory.clear();
}

void Model::ShowControlWindow(Graphics& Gfx, const std::string& modelName) noexcept
{
	if(ImGui::Begin(modelName.c_str()))
	{
		for (size_t iMesh = 0U; iMesh < meshesPtrs.size(); iMesh++)
		{
			std::string hash = modelName + std::to_string(iMesh);
			meshesPtrs[iMesh]->ShowMeshGUI(Gfx, std::move(hash));
		}
	}
	ImGui::End();
}

void Model::Draw(Graphics& Gfx)
{
	for (size_t i = 0; i < meshesPtrs.size(); i++)
	{
		meshesPtrs[i]->Draw(Gfx);
	}
}

Model& Model::Translate(float dx, float dy, float dz) noexcept
{
	for (auto& pMesh : meshesPtrs)
	{
		pMesh->Translate(dx, dy, dz);
	}
	return *this;
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

std::unique_ptr<Mesh> Model::ProccesMesh(Graphics& Gfx, aiMesh* pMesh, size_t materialIndx)
{
	std::vector<std::unique_ptr<IBindable>> bindablePtrs;

	Material* pMat = Gfx.GetMaterialSystem().GetMaterialAt(materialIndx);
	assert(pMat);
	
	bindablePtrs.push_back(std::make_unique<Material>(*pMat));

	const bool HasDiffuseMaps		= pMat->GetMapLayout().hasDiffuseMap;
	const bool HasNormalMaps		= pMat->GetMapLayout().hasNormalMap;
	const bool HasSpecularMaps		= pMat->GetMapLayout().hasSpecularMap;
	const bool HasHeightMaps		= pMat->GetMapLayout().hasHeightMap;
	const bool HasAnyMaps			= HasDiffuseMaps || HasNormalMaps || HasSpecularMaps || HasHeightMaps;

	if (HasAnyMaps)
		bindablePtrs.push_back(std::make_unique<Sampler>(Gfx));

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


	return std::make_unique<Mesh>(Gfx,std::move(bindablePtrs), materialIndx);
}