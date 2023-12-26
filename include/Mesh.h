#pragma once
#include "Graphics.h"
#include "HPipelineElements.h"
#include "Material.h"
#include "imgui.h"
#include <DirectXMath.h>

struct alignas(16) MeshDesc
{
	dx::XMFLOAT4 albedoColor = { 0.f,1.f,0.f, 1.f}; // default color if diffuse map is not used is // WHITE //
	uint32_t matId = 0;
	alignas(4) bool useNormalMap = false;
	alignas(4) bool useDiffuseMap	= false;
	alignas(4) bool useSpecularMap	= false;
	alignas(4) bool useSpecularMapColored = false;
};

struct alignas(16) MeshDescNotex
{
	dx::XMFLOAT4 albedoColor { 0.f,1.f,0.f,1.f }; // default color if diffuse map is not used is // WHITE //
	uint32_t matId = 0;
};

class Mesh : public Drawable
{
	friend class Model;
	friend class Node;
public:
	Mesh() = default;
	Mesh(Graphics& Gfx, std::vector<std::shared_ptr<IBindable>> pBindables, size_t mId);
	~Mesh() = default;

	bool ShowMeshGUI(Graphics& Gfx, std::string hash) noexcept;
	int  GetMaterialIndex() const noexcept;
	virtual void Draw(Graphics& Gfx, DirectX::FXMMATRIX rusultiveTransform) const;
	virtual DirectX::XMMATRIX GetTransform() const noexcept override;
private:
	mutable dx::XMFLOAT4X4 rusultiveTransform{};
	float		   scale = 1.f;
	std::unique_ptr<MeshDesc>	   pMeshDescTextured;
	std::unique_ptr<MeshDescNotex> pMeshDescUntextured;
};