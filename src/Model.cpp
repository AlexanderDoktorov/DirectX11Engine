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
	pRootNode->ShowContolWindow<ObjBuffDiffNorm>(Gfx);
	for (size_t iCh = 0; iCh < pRootNode->pChildren.size(); iCh++)
	{
		pRootNode->pChildren[iCh]->ShowContolWindow<ObjBuffDiffNorm>(Gfx);
	}
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
	std::vector<Texture2D> textures;

	bool HasDiffuseMaps = false;
	bool HasSpecularMaps = false;
	bool HasNormalMaps = false;
	bool HasHeightMaps = false;

	dx::XMFLOAT3 diffuseColor = { 0.1f,0.1f,0.1f };
	dx::XMFLOAT3 specularColor = { 0.f,0.f,0.f };

	// Load textures
	if (pMesh->mMaterialIndex >= 0)
	{
		// TEST

		Material mat(Gfx, pScene->mMaterials[pMesh->mMaterialIndex], directory);
		aiColor3D* speculcarColor = (aiColor3D*)mat.GetProperty(EMATERIAL_PROPERTY_COLOR_SPECULAR);

		// TEST


		aiMaterial* pMaterial = pScene->mMaterials[pMesh->mMaterialIndex];
		aiString texturePath;

		// 1. diffuse maps
		std::vector<Texture2D> diffuseMaps = loadMaterialTextures(Gfx, pMaterial, aiTextureType_DIFFUSE);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		// 2. specular maps
		std::vector<Texture2D> specularMaps = loadMaterialTextures(Gfx, pMaterial, aiTextureType_SPECULAR);
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		// 3. normal maps
		std::vector<Texture2D> normalMaps = loadMaterialTextures(Gfx, pMaterial, aiTextureType_NORMALS);
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
		// 4. height maps
		std::vector<Texture2D> heightMaps = loadMaterialTextures(Gfx, pMaterial, aiTextureType_HEIGHT);
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

		std::unique_ptr<TextureArrayPS> diffuseMapsPS	= std::make_unique<TextureArrayPS>(Gfx,  SLOT_TEXTURE_ARRAY_DIFFUSE);
		std::unique_ptr<TextureArrayPS> normalMapsPS	= std::make_unique<TextureArrayPS>(Gfx,  SLOT_TEXTURE_ARRAY_NORMALMAP);
		std::unique_ptr<TextureArrayPS> specularMapsPS	= std::make_unique<TextureArrayPS>(Gfx, SLOT_TEXTURE_ARRAY_SPECULAR);
		std::unique_ptr<TextureArrayPS> heightMapsPS	= std::make_unique<TextureArrayPS>(Gfx,   SLOT_TEXTURE_ARRAY_HEIGHT);
		// Loop through textures
		for (size_t i = 0; i < textures.size(); i++)
		{
			if (textures[i].textureType == aiTextureType_NORMALS)
			{
				normalMapsPS->PushBack(Gfx, textures[i].textureFilePath.c_str(), DirectX::WIC_LOADER_IGNORE_SRGB);
				HasNormalMaps = true;
			}

			if (textures[i].textureType == aiTextureType_DIFFUSE)
			{
				diffuseMapsPS->PushBack(Gfx, textures[i].textureFilePath.c_str());
				HasDiffuseMaps = true;
			}
			// Get default diffuse color (when diffuse map is not used)
			pMaterial->Get( AI_MATKEY_COLOR_DIFFUSE, reinterpret_cast<aiColor3D&>(diffuseColor));

			if (textures[i].textureType == aiTextureType_SPECULAR)
			{
				specularMapsPS->PushBack(Gfx, textures[i].textureFilePath.c_str());
				HasSpecularMaps = true;
			}
			if(pMaterial->mProperties)
			// Get default specular color (when specular map is not used)
			pMaterial->Get( AI_MATKEY_COLOR_SPECULAR,reinterpret_cast<aiColor3D&>(specularColor) );

			if (textures[i].textureType == aiTextureType_HEIGHT)
			{
				heightMapsPS->PushBack(Gfx, textures[i].textureFilePath.c_str());
				HasHeightMaps = true;
			}

			if (HasDiffuseMaps || HasSpecularMaps || HasNormalMaps || HasHeightMaps)
			{
				bindablePtrs.push_back(std::make_unique<Sampler>(Gfx));
			}
		}
		if(!diffuseMapsPS->Empty())
			bindablePtrs.push_back(std::move(diffuseMapsPS));
		if(!normalMapsPS->Empty())
			bindablePtrs.push_back(std::move(normalMapsPS));
		if(!specularMapsPS->Empty())
			bindablePtrs.push_back(std::move(specularMapsPS));
		if(!heightMapsPS->Empty())
			bindablePtrs.push_back(std::move(heightMapsPS));
	}
	
	if (HasDiffuseMaps && HasNormalMaps && HasSpecularMaps)
	{
		// TO DO
	}
	else if (HasDiffuseMaps && HasNormalMaps)
	{
		DynamicVertex::VertexBuffer vb( std::move(
			DynamicVertex::VertexLayout{}
			.Append( DynamicVertex::VertexLayout::Position3D )
			.Append( DynamicVertex::VertexLayout::Normal )
			.Append( DynamicVertex::VertexLayout::Texture2D )
			.Append( DynamicVertex::VertexLayout::Tangent )
			.Append( DynamicVertex::VertexLayout::Bitangent )
		) );

		for( unsigned int i = 0; i < pMesh->mNumVertices; i++ )
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
		bindablePtrs.push_back( std::make_unique<VertexBuffer>(Gfx, vb));
		bindablePtrs.push_back( std::make_unique<InputLayout>(Gfx, vb.GetLayout().GetD3DLayout(), (IShader*)VS.get()) );
		bindablePtrs.push_back( std::move( VS ) );
		bindablePtrs.push_back( std::make_unique<PixelShaderCommon>(Gfx, L"shaders\\NormalTexturePS.cso") );

		ObjBuffDiffNorm cbuffer{};
		cbuffer.diffuseColor = diffuseColor;
		bindablePtrs.push_back( std::make_unique<PixelConstantBuffer<ObjBuffDiffNorm>>(Gfx, cbuffer, 0U) );
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


	return std::make_unique<Mesh>(Gfx, std::move(bindablePtrs));
}

std::vector<Texture2D> Model::loadMaterialTextures(Graphics& Gfx, aiMaterial* mat, aiTextureType type)
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
			if(std::strcmp(texturesLoaded[j].textureFilePath.data(), fileName.C_Str()) == 0)
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
			texture.textureType = type;
			texture.textureFilePath = fullFilePath;
			textures.push_back(texture);
			texturesLoaded.push_back(texture); // add to loaded textures
		}
	}
	return textures;
}  