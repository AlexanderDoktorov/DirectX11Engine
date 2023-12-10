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
		dx::XMFLOAT3 albedoColor { 1.f,1.f,1.f }; // default color if diffuse map is not used is // WHITE //
	};
public:
	Mesh() = default;
	Mesh(Graphics& Gfx, std::vector<std::unique_ptr<IBindable>> pBindables, size_t matIndx);

	void ShowMeshControls(Graphics& Gfx) noexcept;
	int  GetMaterialIndex() const noexcept;
	virtual DirectX::XMMATRIX GetTransform() const noexcept override;
	Mesh& Translate(float dx, float dy, float dz) noexcept;
private:
	dx::XMFLOAT3 worldTranslation{};
	dx::XMFLOAT3 worldRotation{};
	MeshDesc meshDesc{};
};