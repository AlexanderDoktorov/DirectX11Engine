#pragma once
#include "Graphics.h"
#include "HPipelineElements.h"
#include "Material.h"
#include "imgui.h"
#include <DirectXMath.h>
#include <vector>

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
	Mesh(Graphics& Gfx, std::vector<std::unique_ptr<IBindable>> pBindables, size_t matIndx);

	template<class T>
	bool ShowMeshGUI(Graphics& Gfx, std::string hash) noexcept
	{
		auto makeHashed = [](std::string str, const std::string& hash) -> std::string
		{
			return str.append("##").append(hash);
		};

		bool changed = false;

		if constexpr (std::is_same_v<T, MeshDesc>)
		{
			Material* pMeshMaterial = Gfx.GetMaterialSystem().GetMaterialAt(meshDesc.matId);
			MapLayout mapLayout = pMeshMaterial->GetMapLayout();

			if(mapLayout.hasDiffuseMap)
				changed |= ImGui::Checkbox(makeHashed("Use diffuse map", hash).c_str(), &meshDesc.useDiffuseMap);
			if(!meshDesc.useDiffuseMap)
				changed |= ImGui::ColorEdit4(makeHashed("Material albedo color", hash).c_str(), &meshDesc.albedoColor.x);
			if(mapLayout.hasNormalMap)
				changed |= ImGui::Checkbox(makeHashed("Use normal map", hash).c_str(), &meshDesc.useNormalMap);
			if(mapLayout.hasSpecularMap)
				changed |= ImGui::Checkbox(makeHashed("Use specular map", hash).c_str(), &meshDesc.useSpecularMap);

			if(auto pMeshBuffer = QueryBindable<meshBufferTextured>(); changed && pMeshBuffer)
				pMeshBuffer->Update(Gfx, meshDesc);
		}
		if constexpr (std::is_same_v<T, MeshDescNotex>)
		{
			changed |= ImGui::ColorEdit4(makeHashed("Material albedo color", hash).c_str(), &meshDesc.albedoColor.x);
			if (auto pMeshBufferNoTex = QueryBindable<meshBufferNoTex>(); changed && pMeshBufferNoTex)
				pMeshBufferNoTex->Update(Gfx, MeshDescNotex{ meshDesc.albedoColor, meshDesc.matId });
		}
		return changed;
	}
	int  GetMaterialIndex() const noexcept;
	virtual DirectX::XMMATRIX GetTransform() const noexcept override;
	Mesh& Translate(float dx, float dy, float dz) noexcept;
	Mesh& SetPosition(float x, float y, float z)  noexcept;
private:
	dx::XMFLOAT3 worldTranslation{};
	dx::XMFLOAT3 worldRotation{};
	MeshDesc meshDesc{};
};