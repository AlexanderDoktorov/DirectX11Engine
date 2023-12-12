#pragma once
#include "Drawable.h"
#include "Node.h"
#include "Mesh.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/Scene.h"
#include "Material.h"
#include "Numerated.h"

class Model : public Drawable
{
public:
	Model() = default;
	Model(Graphics& Gfx, const std::string& fileName, unsigned int aippFlags);
	void Load(Graphics& Gfx, const std::string& fileName, unsigned int aippFlags) noexcept;
	void ClearData() noexcept;
	void ShowControlWindow(Graphics& Gfx, const std::string& modelName) noexcept;
	virtual void Draw(Graphics& Gfx) override;
	Model& Translate(float dx, float dy, float dz) noexcept;
	Model& SetPostion(float x, float y, float z) noexcept;
	Model& SetRotation(float roll, float pitch, float yaw) noexcept;
private:
	std::unique_ptr<Node>  ProcessNode(Graphics& Gfx, int& startID, aiNode* pRootNode);
	std::unique_ptr<Mesh>  ProccesMesh(Graphics& Gfx, aiMesh* pMesh, size_t materialIndx);
private:
	std::vector<std::shared_ptr<Mesh>>		meshesPtrs;
	std::vector<size_t>						materialsIndices;
	std::unique_ptr<Node>			        pRootNode;
	DirectX::XMFLOAT3						modelRotation{};
	std::string directory;
};