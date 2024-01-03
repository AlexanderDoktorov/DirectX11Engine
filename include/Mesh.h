#pragma once
#include "Graphics.h"
#include "HPipelineElements.h"
#include "Material.h"
#include "imgui.h"
#include <assimp\mesh.h>
#include <DirectXMath.h>

class Mesh : public Drawable
{
	friend class Model;
	friend class Node;
public:
	Mesh(Graphics& Gfx, const aiMesh* p_Mesh, const aiMaterial* p_aiMaterial, std::string directory);

	Mesh() = default;
	Mesh(Mesh&& other) = default;
	Mesh& operator=(Mesh&& other) = default;
	~Mesh() = default;

	static std::unique_ptr<Mesh> Load(Graphics& Gfx, const aiMesh* p_Mesh, const aiMaterial* p_aiMaterial, std::string directory); 

	bool ShowMeshGUI(Graphics& Gfx, std::string hash) noexcept;
	virtual void Draw(Graphics& Gfx, DirectX::FXMMATRIX rusultiveTransform) const;
	virtual DirectX::XMMATRIX GetTransform() const noexcept override;
private:
	mutable dx::XMFLOAT4X4 rusultiveTransform{};
	std::shared_ptr<Material> p_MeshMaterial;
	float		   scale = 1.f;
};