#include "Model.h"
#include <ranges>

// ********** Model **********

Model::Model(Graphics& Gfx, const std::string& fileName, unsigned int aippFlags)
{
	Assimp::Importer imp;
	const aiScene* pScene = imp.ReadFile(fileName.c_str(), aippFlags);

	if (!pScene || pScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !pScene->mRootNode) // if is Not Zero
		throw assimpException(imp.GetErrorString(), __LINE__, __FILE__);

	directory = fileName.substr(0, fileName.find_last_of('\\'));

	// Fill materialPtrs array with all scene materials and push them to structured buffer
	m_Indices.reserve(pScene->mNumMaterials);
	for (size_t i = 0; i < pScene->mNumMaterials; i++)
	{
		m_Indices.push_back(Gfx.GetMaterialSystem().ResolveMaterial(Gfx, pScene->mMaterials[i], directory));
	}

	// Fill meshesPtrs array with all scene meshes
	m_MeshesPtrs.reserve(pScene->mNumMeshes);
	for (size_t i = 0; i < pScene->mNumMeshes; i++) {
		m_MeshesPtrs.emplace_back(Mesh::Load(Gfx, pScene->mMeshes[i], m_Indices[pScene->mMeshes[i]->mMaterialIndex]));
	}

	int StartId = 0;
	pRootNode = ProcessNode(Gfx, StartId, pScene->mRootNode);
}

Model::Model(Model&& other) noexcept
{
	position		 = std::move(other.position);
	rotation		 = std::move(other.rotation);
	directory		 = std::move(other.directory);
	m_Indices		 = std::move(other.m_Indices);
	m_MeshesPtrs	 = std::move(other.m_MeshesPtrs);
	pRootNode		 = std::move(other.pRootNode);
}

Model Model::Load(Graphics& Gfx, const std::string& fileName, unsigned int aippFlags) noexcept
{
	return Model(Gfx, fileName, aippFlags);
}

void Model::ClearData() noexcept
{
	m_MeshesPtrs.clear();
	m_Indices.clear();
	if (pRootNode)
		pRootNode->Clear();
	directory.clear();
}

void Model::ShowControlWindow(Graphics& Gfx, const std::string& modelName) noexcept
{
	if (ImGui::Begin(modelName.c_str()))
	{
		using namespace std::string_literals;
		size_t iMesh = 0ull;
		std::ranges::for_each(m_MeshesPtrs, [&](auto& mesh) {
			mesh->ShowMeshGUI(Gfx, std::string(modelName) + std::to_string(iMesh++)); 
		});
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
		dx::XMMatrixRotationRollPitchYaw(rotation_.y, rotation_.z, rotation_.x) * 
		dx::XMMatrixTranslation(translation_.x, translation_.y,translation_.z)
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
		nodeMeshes.push_back(m_MeshesPtrs.at(meshIndx).get());
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