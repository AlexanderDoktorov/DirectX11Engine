#include "Model.h"

// ********** Model **********

Model::Model(Graphics& Gfx, const std::string& fileName, unsigned int aippFlags)
{
	Load(Gfx, fileName, aippFlags);
}

Model::Model(Model&& other) noexcept
{
	position		 = std::move(other.position);
	rotation		 = std::move(other.rotation);
	directory		 = std::move(other.directory);
	materialsIndices = std::move(other.materialsIndices);
	meshesPtrs		 = std::move(other.meshesPtrs);
	pRootNode		 = std::move(other.pRootNode);
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
		materialsIndices.push_back(Gfx.GetMaterialSystem().ResolveMaterial(Gfx, pScene->mMaterials[i], directory));
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
		dx::XMFLOAT3& rotationRef = rotation;
		dx::XMFLOAT3& positionRef = position;
		if (trackRot)
			rotationRef = *trackRot;
		if (trackPos)
			positionRef = *trackPos;
		
		ImGui::SliderAngle(("Roll"s + modelName).c_str(), &rotationRef.x);
		ImGui::SliderAngle(("Pitch"s + modelName).c_str(), &rotationRef.y);
		ImGui::SliderAngle(("Yaw"s + modelName).c_str(), &rotationRef.z);
		ImGui::SliderFloat(("Position x"s + modelName).c_str(), &positionRef.x, -100.f, 100.f);
		ImGui::SliderFloat(("Position y"s + modelName).c_str(), &positionRef.y, -100.f, 100.f);
		ImGui::SliderFloat(("Position z"s + modelName).c_str(), &positionRef.z, -100.f, 100.f);
	}		
	ImGui::End();
}

void Model::Draw(Graphics& Gfx) const noexcept
{
	dx::XMFLOAT3 translation_ = position;
	dx::XMFLOAT3 rotation_    = rotation;
	if (trackRot)
		rotation_ = *trackRot;
	if (trackPos)
		translation_ = *trackPos;

	pRootNode->Draw(Gfx, 
		dx::XMMatrixTranslation(translation_.x, translation_.y,translation_.z) *
		dx::XMMatrixRotationRollPitchYaw(rotation_.y, rotation_.z, rotation_.x)
	);
}

Model& Model::SetRootTransform( DirectX::FXMMATRIX tf ) noexcept
{
	pRootNode->SetAppliedTransform( tf );
	return *this;
}

void Model::TrackPosition(dx::XMFLOAT3& posToTrack) noexcept
{
	trackPos = &posToTrack;
}

void Model::TrackRotation(dx::XMFLOAT3& rotToTrack) noexcept
{
	trackRot = &rotToTrack;
}

void Model::UntrackPosition() noexcept
{
	trackPos = nullptr;
}

void Model::UntrackRotation() noexcept
{
	trackRot = nullptr;
}

std::unique_ptr<Node> Model::ProcessNode(Graphics& Gfx, int& startID, aiNode* pRootNode)
{
	const dx::XMMATRIX mDxNodeTransform = dx::XMMatrixTranspose( dx::XMLoadFloat4x4(
		reinterpret_cast<const dx::XMFLOAT4X4*>(&pRootNode->mTransformation)
	) );
	// fill nodeMeshes pointers for building a node
	std::vector<Mesh*> nodeMeshes;
	nodeMeshes.reserve(pRootNode->mNumMeshes);
	for(unsigned int i = 0; i < pRootNode->mNumMeshes; i++)
	{
		const unsigned int meshIndx = pRootNode->mMeshes[i];
		nodeMeshes.push_back(meshesPtrs.at(meshIndx).get());
	}

	// construct root with meshes and transformation
	std::unique_ptr<Node> pMyNode = std::make_unique<Node>(startID++, pRootNode->mName.C_Str(), nodeMeshes, mDxNodeTransform);
	
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
	std::shared_ptr<Material> pMat = Gfx.GetMaterialSystem().GetMaterialByIndex(mId);
	assert(pMat);
	// Add material as bindable (binds material textures to pipeline when drawing mesh) if has some
	bindablePtrs.push_back(pMat);

	const bool HasDiffuseMaps		= pMat->GetMapLayout().hasDiffuseMap;
	const bool HasNormalMaps		= pMat->GetMapLayout().hasNormalMap;
	const bool HasSpecularMaps		= pMat->GetMapLayout().hasSpecularMap || pMat->GetMapLayout().hasSpecularMapColored;
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

	std::string meshTag = directory + "%" + pMesh->mName.C_Str();
	bindablePtrs.push_back( IndexBuffer::Resolve(Gfx, meshTag, indices) );


	return std::make_unique<Mesh>(Gfx, std::move(bindablePtrs), mId);
}