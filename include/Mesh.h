#pragma once
#include "Graphics.h"
#include "HPipelineElements.h"
#include "Material.h"
#include "imgui.h"
#include <DirectXMath.h>
#include <vector>
#include <variant>

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
	virtual void Draw(Graphics& Gfx) override;
	virtual DirectX::XMMATRIX GetTransform() const noexcept override;
	Mesh& Translate(float dx, float dy, float dz) noexcept;
	Mesh& SetPosition(float x, float y, float z)  noexcept;
	Mesh& SetRotatin(float roll, float pitch, float yaw)  noexcept;
	Mesh& Scale(float scaleXYZ)  noexcept;
private:
	dx::XMFLOAT3 worldTranslation{};
	dx::XMFLOAT3 worldRotation{};
	float		 scale = 1.f;
	std::unique_ptr<MeshDesc>	   pMeshDescTextured;
	std::unique_ptr<MeshDescNotex> pMeshDescUntextured;
};