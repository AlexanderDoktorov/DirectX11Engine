#include "Model.h"

// ********** Model **********

Model::Model(Graphics& Gfx, const std::string& fileName, unsigned int aippFlags)
{
	Load(Gfx, fileName, aippFlags);
}

void Model::Load(Graphics& Gfx, const std::string& fileName, unsigned int aippFlags) noexcept
{
	ClearData();

	Assimp::Importer imp;
	const aiScene* pScene = imp.ReadFile(fileName.c_str(), aippFlags);

	if (!pScene || pScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !pScene->mRootNode) // if is Not Zero
	{
		OutputDebugStringA("ERROR ASSIMP: ");
		OutputDebugStringA(imp.GetErrorString());
		return;
	}

	directory = fileName.substr(0, fileName.find_last_of('\\'));

	// Fill materialPtrs array with all scene materials and push them to structured buffer
	materialsIndices.reserve(pScene->mNumMaterials);
	for (size_t i = 0; i < pScene->mNumMaterials; i++)
	{
		materialsIndices.push_back(Gfx.GetMaterialSystem().GetMaterialIndex(pScene->mMaterials[i], directory));
	}

	// Fill meshesPtrs array with all scene meshes
	meshesPtrs.reserve(pScene->mNumMeshes);
	for (size_t i = 0; i < pScene->mNumMeshes; i++)
	{
		meshesPtrs.push_back(ProccesMesh(Gfx, pScene->mMeshes[i], materialsIndices[pScene->mMeshes[i]->mMaterialIndex]));
	}

	int StartId = 0;
	pRootNode = ProcessNode(Gfx, StartId, pScene->mRootNode);
}

void Model::ClearData() noexcept
{
	meshesPtrs.clear();
	materialsIndices.clear();
	if (pRootNode)
		pRootNode->Clear();
	directory.clear();
}

void Model::ShowControlWindow(Graphics& Gfx, const std::string& modelName) noexcept
{
	if (ImGui::Begin(modelName.c_str()))
	{
		using namespace std::string_literals;
		for (size_t iMesh = 0U; iMesh < meshesPtrs.size(); iMesh++)
		{
			std::string hash = modelName + std::to_string(iMesh);
			meshesPtrs[iMesh]->ShowMeshGUI(Gfx, std::move(hash));
		}

		bool rotated = false;
		rotated |= ImGui::SliderAngle(("Roll"s + modelName).c_str(), &modelRotation.x);
		rotated |= ImGui::SliderAngle(("Pirch"s + modelName).c_str(), &modelRotation.y);
		rotated |= ImGui::SliderAngle(("Yaw"s + modelName).c_str(), &modelRotation.z);
		if(rotated)
			std::for_each(meshesPtrs.begin(), meshesPtrs.end(), [&](const std::shared_ptr<Mesh>& pMesh) { pMesh->SetRotatin(modelRotation.x, modelRotation.y, modelRotation.z); });
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

Model& Model::SetPostion(float x, float y, float z) noexcept
{
	for (auto& pMesh : meshesPtrs)
	{
		pMesh->SetPosition(x, y, z);
	}
	return *this;
}

Model& Model::SetRotation(float roll, float pitch, float yaw) noexcept
{
	for (auto& pMesh : meshesPtrs)
		pMesh->SetRotatin(roll, pitch, yaw);
	return *this;
}

Model& Model::Scale(float scaleXYZ) noexcept
{
	for (auto& pMesh : meshesPtrs)
		pMesh->Scale(scaleXYZ);
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

std::unique_ptr<Mesh> Model::ProccesMesh(Graphics& Gfx, aiMesh* pMesh, size_t mId) const
{
	std::vector<std::shared_ptr<IBindable>> bindablePtrs;
	std::shared_ptr<VertexShaderCommon> vertexShader = nullptr;
	DynamicVertex::VertexBuffer vertexBuffer;

	// Get material index from material system in gfx (or create new and get index)
	std::shared_ptr<Material> pMat = Gfx.GetMaterialSystem().GetMaterialAt(mId);
	assert(pMat);
	// Add material as bindable (binds material textures to pipeline when drawing mesh) if has some
	bindablePtrs.push_back(pMat);
	
	std::string meshTag = directory + "%" + pMesh->mName.C_Str();

	const bool HasDiffuseMaps		= pMat->GetMapLayout().hasDiffuseMap;
	const bool HasNormalMaps		= pMat->GetMapLayout().hasNormalMap;
	const bool HasSpecularMaps		= pMat->GetMapLayout().hasSpecularMap;
	const bool HasHeightMaps		= pMat->GetMapLayout().hasHeightMap;
	const bool HasAnyMaps			= HasDiffuseMaps || HasNormalMaps || HasSpecularMaps || HasHeightMaps;

	if (HasAnyMaps)
		bindablePtrs.push_back(std::make_unique<Sampler>(Gfx));

	if (!HasAnyMaps)
	{
		vertexBuffer = DynamicVertex::VertexBuffer(std::move(
			DynamicVertex::VertexLayout{}
			.Append(DynamicVertex::VertexLayout::Position3D)
			.Append(DynamicVertex::VertexLayout::Normal)
		));
		for (unsigned int i = 0; i < pMesh->mNumVertices; i++)
		{
			vertexBuffer.EmplaceBack(
				*(reinterpret_cast<DirectX::XMFLOAT3*>(&pMesh->mVertices[i])),
				*(reinterpret_cast<DirectX::XMFLOAT3*>(&pMesh->mNormals[i]))
			);
		}

		vertexShader = VertexShaderCommon::Resolve(Gfx, L"shaders\\NotextureVS.cso");
		bindablePtrs.push_back(PixelShaderCommon::Resolve(Gfx, L"shaders\\NotexturePS.cso"));
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

		for (unsigned int i = 0; i < pMesh->mNumVertices; i++)
		{
			vertexBuffer.EmplaceBack(
				*(reinterpret_cast<DirectX::XMFLOAT3*>(&pMesh->mVertices[i])),
				*(reinterpret_cast<DirectX::XMFLOAT3*>(&pMesh->mNormals[i])),
				*(reinterpret_cast<DirectX::XMFLOAT2*>(&pMesh->mTextureCoords[0][i])),
				*(reinterpret_cast<DirectX::XMFLOAT3*>(&pMesh->mTangents[i])),
				*(reinterpret_cast<DirectX::XMFLOAT3*>(&pMesh->mBitangents[i]))
			);
		}

		vertexShader = VertexShaderCommon::Resolve(Gfx, L"shaders\\NormalTextureVS.cso");
		bindablePtrs.push_back(PixelShaderCommon::Resolve(Gfx, L"shaders\\NormalTexturePS.cso"));
	}

	bindablePtrs.push_back(std::make_unique<VertexBuffer>(Gfx, vertexBuffer));
	bindablePtrs.push_back(InputLayout::Resolve(Gfx, vertexBuffer.GetLayout(), (IShader*)vertexShader.get()));
	bindablePtrs.push_back(std::move(vertexShader));

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
	bindablePtrs.push_back( IndexBuffer::Resolve(Gfx, meshTag, indices) );


	return std::make_unique<Mesh>(Gfx, std::move(bindablePtrs), mId);
}