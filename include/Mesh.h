#pragma once
#include "Graphics.h"
#include "HPipelineElements.h"
#include "Material.h"
#include "imgui.h"
#include <DirectXMath.h>
#include <vector>
#include <variant>

class Mesh : public Drawable
{
	friend class Model;
	friend class Node;
public:
	struct alignas(16) MeshDesc
	{
		bool useNormalMap	= false;
		bool pad1[3];
		bool useDiffuseMap	= false;
		bool pad2[3];
		bool useSpecularMap	= false;
		bool pad3[3];
		int matId;
		dx::XMFLOAT4 albedoColor { 1.f,1.f,1.f, 1.f}; // default color if diffuse map is not used is // WHITE //
	};
	struct alignas(16) MeshDescNotex
	{
		dx::XMFLOAT4 albedoColor { 1.f,1.f,1.f,1.f }; // default color if diffuse map is not used is // WHITE //
		int matId;
	};
	typedef PixelConstantBuffer<MeshDesc> meshBufferTextured;
	typedef PixelConstantBuffer<MeshDescNotex> meshBufferNoTex;
public:
	Mesh() = default;
	Mesh(Graphics& Gfx, std::vector<std::shared_ptr<IBindable>> pBindables, size_t mId);

	bool ShowMeshGUI(Graphics& Gfx, std::string hash) noexcept;
	int  GetMaterialIndex() const noexcept;
	virtual DirectX::XMMATRIX GetTransform() const noexcept override;
	Mesh& Translate(float dx, float dy, float dz) noexcept;
	Mesh& SetPosition(float x, float y, float z)  noexcept;
	Mesh& SetRotatin(float roll, float pitch, float yaw)  noexcept;
private:
	dx::XMFLOAT3 worldTranslation{};
	dx::XMFLOAT3 worldRotation{};
	std::variant<MeshDesc, MeshDescNotex> meshDesc;
};