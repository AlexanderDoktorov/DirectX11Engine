#pragma once
#include "Drawable.h"
#include "Mesh.h"

class Model : public Drawable
{
public:
	Model() = default;
	Model(Graphics& Gfx, const std::string& fileName, unsigned int aippFlags);
	void ShowControlWindow() noexcept;
	virtual void Draw(Graphics& Gfx) override;
private:
	void ProcessNode(Graphics& Gfx, aiNode* pNode, const aiScene* pScene);
	std::unique_ptr<Mesh> ProccesMesh(Graphics& Gfx, aiMesh* pMesh, const aiScene* pScene);
	std::vector<Texture2D> loadMaterialTextures(Graphics& Gfx, aiMaterial* mat, aiTextureType type, std::string typeName);
	std::vector<std::unique_ptr<Mesh>> meshesPtrs;
	std::vector<Texture2D> texturesLoaded;
	std::string directory;
	struct CBUFF
	{
		alignas(16) bool nmEnabled = false;
	} objCBuff;
};