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

	bool ShowMeshGUI(Graphics& Gfx, std::string hash) noexcept
	{
		auto makeHashed = [](std::string str, const std::string& hash) -> std::string
		{
			return str.append("##").append(hash);
		};
		bool changed = false;
		if (auto pDesc = std::get_if<MeshDesc>(&meshDesc))
		{
			auto pMeshMaterial = Gfx.GetMaterialSystem().GetMaterialAt(pDesc->matId);
			MapLayout mapLayout = pMeshMaterial->GetMapLayout();

			if(mapLayout.hasDiffuseMap)
				changed |= ImGui::Checkbox(makeHashed("Use diffuse map", hash).c_str(), &pDesc->useDiffuseMap);
			if(!pDesc->useDiffuseMap)
				changed |= ImGui::ColorEdit4(makeHashed("Material albedo color", hash).c_str(), &pDesc->albedoColor.x);
			if(mapLayout.hasNormalMap)
				changed |= ImGui::Checkbox(makeHashed("Use normal map", hash).c_str(), &pDesc->useNormalMap);
			if(mapLayout.hasSpecularMap)
				changed |= ImGui::Checkbox(makeHashed("Use specular map", hash).c_str(), &pDesc->useSpecularMap);

			if(auto pMeshBuffer = QueryBindable<meshBufferTextured>(); changed && pMeshBuffer)
				pMeshBuffer->Update(Gfx, *pDesc);
		}
		if (auto pNoTexDesc = std::get_if<MeshDescNotex>(&meshDesc))
		{
			changed |= ImGui::ColorEdit4(makeHashed("Material albedo color", hash).c_str(), &pNoTexDesc->albedoColor.x);
			if (auto pMeshBufferNoTex = QueryBindable<meshBufferNoTex>(); changed && pMeshBufferNoTex)
				pMeshBufferNoTex->Update(Gfx, *pNoTexDesc);
		}
		return changed;
	}
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