#pragma once
#include "Drawable.h"
#include "Node.h"
#include "Mesh.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/Scene.h"
#include "Material.h"
#include "Numerated.h"
#include "baseException.h"

class Model : public Drawable
{
	class assimpException : public baseException
	{
	public:
		assimpException(const char* errorString, int codeLine, const char* codeFileName)
			: 
			baseException(codeLine, codeFileName),
			errorString(errorString) {}

		virtual std::string what() const override {
			std::stringstream ss;
			ss << "ERROR ASSIMP:  " << errorString << std::endl << baseException::what() << std::endl;
			return ss.str().c_str();
		}
	private:
		const char* errorString;
	};
public:
	Model() = default;
	Model(Graphics& Gfx, const std::string& fileName, unsigned int aippFlags);
	Model(Model&& other) noexcept;
	bool Load(Graphics& Gfx, const std::string& fileName, unsigned int aippFlags) noexcept; 
	void ClearData() noexcept;
	void ShowControlWindow(Graphics& Gfx, const std::string& modelName) noexcept;
	virtual void Draw(Graphics& Gfx) const noexcept override;
	Model& SetRootTransform(DirectX::FXMMATRIX tf) noexcept;
protected:
	void TrackPosition(dx::XMFLOAT3& posToTrack) noexcept;
	void TrackRotation(dx::XMFLOAT3& rotToTrack) noexcept;
	void UntrackPosition() noexcept;
	void UntrackRotation() noexcept;
protected:
	std::unique_ptr<Node>  ProcessNode(Graphics& Gfx, int& startID, aiNode* pRootNode);
	std::unique_ptr<Mesh>  ProccesMesh(Graphics& Gfx, aiMesh* pMesh, size_t mId) const;
private:
	const dx::XMFLOAT3* trackPos = nullptr;
	const dx::XMFLOAT3* trackRot = nullptr;
	dx::XMFLOAT3  position = dx::XMFLOAT3(0.f,0.f,0.f);
	dx::XMFLOAT3  rotation = dx::XMFLOAT3(0.f,0.f,0.f);
	std::string directory;
	std::vector<size_t>						materialsIndices;
	std::vector<std::shared_ptr<Mesh>>		meshesPtrs;
	std::unique_ptr<Node>			        pRootNode;
};