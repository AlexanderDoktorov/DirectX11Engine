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
	virtual void Draw(Graphics& Gfx) const noexcept override;
	Model& SetRootTransform(DirectX::FXMMATRIX tf) noexcept;
protected:
	std::unique_ptr<Node>  ProcessNode(Graphics& Gfx, int& startID, aiNode* pRootNode);
	std::unique_ptr<Mesh>  ProccesMesh(Graphics& Gfx, aiMesh* pMesh, size_t mId) const;
	struct TransformData
	{
		float x = 0.f;
		float y = 0.f;
		float z = 0.f;
		float pitch = 0.f;
		float roll = 0.f;
		float yaw = 0.f;
	} transformData{};
private:
	std::string directory;
	std::vector<size_t>						materialsIndices;
	std::vector<std::shared_ptr<Mesh>>		meshesPtrs;
	std::unique_ptr<Node>			        pRootNode;
};