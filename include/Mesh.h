#pragma once
#include "Graphics.h"
#include "HPipelineElements.h"
#include "Material.h"
#include "imgui.h"
#include <assimp\mesh.h>
#include <DirectXMath.h>

struct alignas(16) MeshDesc
{
	dx::XMFLOAT4 albedoColor = { 0.f,1.f,0.f, 1.f}; // default color if diffuse map is not used is // RED //
	int matId = -1; // invalid value as default 
	alignas(4) bool useNormalMap = false;
	alignas(4) bool useDiffuseMap	= false;
	alignas(4) bool useHeightMap = false;
	alignas(4) bool useSpecularMap	= false;
	alignas(4) bool useSpecularAlpha = false;
};

class Mesh : public Drawable
{
	friend class Model;
	friend class Node;
public:
	Mesh(Graphics& Gfx, aiMesh* p_Mesh, size_t v_id);

	Mesh() = default;
	Mesh(Mesh&& other) = default;
	Mesh& operator=(Mesh&& other) = default;
	~Mesh() = default;

	static std::unique_ptr<Mesh> Load(Graphics& Gfx, aiMesh* p_Mesh, size_t v_id); // TO DO

	bool ShowMeshGUI(Graphics& Gfx, std::string hash) noexcept;
	int  GetMaterialIndex() const noexcept;
	virtual void Draw(Graphics& Gfx, DirectX::FXMMATRIX rusultiveTransform) const;
	virtual DirectX::XMMATRIX GetTransform() const noexcept override;
private:
	mutable dx::XMFLOAT4X4 rusultiveTransform{};
	float		   scale = 1.f;
	MeshDesc	   v_MeshDesc{};
};