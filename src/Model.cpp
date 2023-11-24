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

	// process ASSIMP's root node recursively
	ProcessNode(Gfx, pScene->mRootNode, pScene);
}

void Model::ShowControlWindow() noexcept
{
	if (ImGui::Begin("Model Options"))
	{
		ImGui::Checkbox("Enable normal map",  &objCBuff.nmEnabled);
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

void Model::ProcessNode(Graphics& Gfx, aiNode* pNode, const aiScene* pScene)
{
	// process all the node's meshes (if any)
	for(unsigned int i = 0; i < pNode->mNumMeshes; i++)
	{
		aiMesh* pMesh = pScene->mMeshes[pNode->mMeshes[i]]; 
		meshesPtrs.push_back(ProccesMesh(Gfx, pMesh, pScene));
	}
	// then do the same for each of its children
	for(unsigned int i = 0; i < pNode->mNumChildren; i++)
	{
		ProcessNode(Gfx, pNode->mChildren[i], pScene);
	}
}

std::unique_ptr<Mesh> Model::ProccesMesh(Graphics& Gfx, aiMesh* pMesh, const aiScene* pScene)
{
	typedef DynamicVertex::VertexLayout::ElementType VParam;
	DynamicVertex::VertexBuffer vb(DynamicVertex::VertexLayout{}
		.Append(VParam::Position3D)
		.Append(VParam::Texture2D)
		.Append(VParam::Normal)
		.Append(VParam::Tangent)
		.Append(VParam::Bitangent)
	);

	for (size_t i = 0; i < pMesh->mNumVertices; i++)
	{
		vb.EmplaceBack(
			*(reinterpret_cast<DirectX::XMFLOAT3*>(&pMesh->mVertices[i])),
			*(reinterpret_cast<DirectX::XMFLOAT2*>(&pMesh->mTextureCoords[0][i])),
			*(reinterpret_cast<DirectX::XMFLOAT3*>(&pMesh->mNormals[i])),
			*(reinterpret_cast<DirectX::XMFLOAT3*>(&pMesh->mTangents[i])),
			*(reinterpret_cast<DirectX::XMFLOAT3*>(&pMesh->mBitangents[i]))
		);
	}

	std::vector<unsigned short> indices;
	indices.reserve(pMesh->mNumFaces * 3);

	// For each face retrieve it's indices 
	// Faces might be any form - not only triangles
	for (size_t i = 0; i < pMesh->mNumFaces; i++)
	{
		for (size_t k = 0; k < pMesh->mFaces[i].mNumIndices; k++)
			indices.push_back(pMesh->mFaces[i].mIndices[k]);
	}

	std::vector<std::unique_ptr<IBindable>> bindablePtrs;
	std::vector<Texture2D> textures;

	if(pMesh->mMaterialIndex >= 0) // Has materials, want to retrieve it from scene
	{
		aiMaterial* pMaterial = pScene->mMaterials[pMesh->mMaterialIndex];

		// 1. diffuse maps
		std::vector<Texture2D> diffuseMaps = loadMaterialTextures(Gfx, pMaterial, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		// 2. specular maps
		std::vector<Texture2D> specularMaps = loadMaterialTextures(Gfx, pMaterial, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		// 3. normal maps
		std::vector<Texture2D> normalMaps = loadMaterialTextures(Gfx, pMaterial, aiTextureType_NORMALS, "texture_normal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
		if (normalMaps.size() > 0)
			objCBuff.nmEnabled = true;
		// 4. height maps
		std::vector<Texture2D> heightMaps = loadMaterialTextures(Gfx, pMaterial, aiTextureType_HEIGHT, "texture_height");
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

		bindablePtrs.push_back(std::make_unique<Sampler>(Gfx));
	}

	for (size_t i = 0; i < textures.size(); i++)
	{
		if (textures[i].type == "texture_normal")
			bindablePtrs.push_back(std::make_unique<PixelShaderPictureTexture>(Gfx, textures[i].path.c_str(), SLOT_TEXTURE_NORMALMAP));
		if (textures[i].type == "texture_diffuse")
			bindablePtrs.push_back(std::make_unique<PixelShaderPictureTexture>(Gfx, textures[i].path.c_str(), SLOT_TEXTURE_DIFFUSE));
		if (textures[i].type == "texture_specular")
			bindablePtrs.push_back(std::make_unique<PixelShaderPictureTexture>(Gfx, textures[i].path.c_str(), SLOT_TEXTURE_SPECULAR));
		if (textures[i].type == "texture_height")
			bindablePtrs.push_back(std::make_unique<PixelShaderPictureTexture>(Gfx, textures[i].path.c_str(), SLOT_TEXTURE_HEIGHT));
	}
	
	auto VS = std::make_unique<VertexShaderCommon>(Gfx, L"shaders\\NormalTextureVS.cso");
	bindablePtrs.push_back( std::make_unique<IndexBuffer>(Gfx, indices));
	bindablePtrs.push_back( std::make_unique<VertexBuffer>(Gfx, vb));
	bindablePtrs.push_back( std::make_unique<InputLayout>(Gfx, vb.GetLayout().GetD3DLayout(), (IShader*)VS.get()) );
	bindablePtrs.push_back( std::move( VS ) );
	bindablePtrs.push_back( std::make_unique<PixelShaderCommon>(Gfx, L"shaders\\NormalTexturePS.cso") );
	bindablePtrs.push_back( std::make_unique<DataBufferPS<CBUFF>>(Gfx, objCBuff, 2U) );

	return std::make_unique<Mesh>(Gfx, std::move(bindablePtrs));
}

std::vector<Texture2D> Model::loadMaterialTextures(Graphics& Gfx, aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture2D> textures;
	textures.reserve(mat->GetTextureCount(type));

	for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString fileName;
		mat->GetTexture(type, i, &fileName);
		bool skip = false;
		for(unsigned int j = 0; j < texturesLoaded.size(); j++)
		{
			if(std::strcmp(texturesLoaded[j].path.data(), fileName.C_Str()) == 0)
			{
				textures.push_back(texturesLoaded[j]);
				skip = true; 
				break;
			}
		}
		if(!skip)
		{   
			std::string fullFilePath =  directory + '\\' + std::string(fileName.C_Str());
			// if texture hasn't been loaded already, load it
			Texture2D texture;
			texture.CreatePictureTexture(Gfx, fullFilePath.c_str());
			texture.type = typeName;
			texture.path = fullFilePath;
			textures.push_back(texture);
			texturesLoaded.push_back(texture); // add to loaded textures
		}
	}
	return textures;
}  