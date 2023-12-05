#pragma once
#include "Drawable.h"
#include "Node.h"
#include "Mesh.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/Scene.h"
#include "Material.h"

class Model : public Drawable
{
public:
	Model() = default;
	Model(Graphics& Gfx, const std::string& fileName, unsigned int aippFlags);
	void ShowControlWindow(Graphics& Gfx) noexcept;
	virtual void Draw(Graphics& Gfx) override;
private:
	std::unique_ptr<Node>  ProcessNode(Graphics& Gfx, int& startID, aiNode* pRootNode);
	std::unique_ptr<Mesh>  ProccesMesh(Graphics& Gfx, aiMesh* pMesh, const aiScene* pScene);
private:
	std::vector<std::shared_ptr<Mesh>>		meshesPtrs;
	std::vector<std::shared_ptr<Material>>  materialsPtrs;
	std::unique_ptr<Node>			        pRootNode;
	std::string directory;
};