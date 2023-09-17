#include "House.h"

House::House(Graphics& Gfx)
{
	if (!DrawableBase::Initilized())
	{
		struct Vertex_
		{
			dx::XMFLOAT3 pos;
			dx::XMFLOAT3 n;
		};

		using type = DynamicVertex::VertexLayout::ElementType;
		DynamicVertex::VertexLayout vl;
		vl.Append(type::Position3D).Append(type::Normal);

		Assimp::Importer importer;
		std::string path = "Models\\cottage_obj.obj";
		auto pScene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);
		const auto pMesh = pScene->mMeshes[0];

		std::vector<Vertex_> vertices;
		vertices.reserve(pMesh->mNumVertices);
		for (unsigned int i = 0; i < pMesh->mNumVertices; i++)
		{
			vertices.push_back({ *(dx::XMFLOAT3*)(&pMesh->mVertices[i]), *(dx::XMFLOAT3*)(&pMesh->mNormals[i]) });
		}
		std::vector<unsigned short> indices;
		indices.reserve(pMesh->mNumFaces * 3);
		for (unsigned int i = 0; i < pMesh->mNumFaces; i++)
		{
			const auto& face = pMesh->mFaces[i];
			assert(face.mNumIndices == 3);
			indices.push_back(face.mIndices[0]);
			indices.push_back(face.mIndices[1]);
			indices.push_back(face.mIndices[2]);
		}

		auto VS = std::make_unique<VertexShaderCommon>(Gfx, L"GeometryVS.cso");
		AddStaticBindable(std::make_unique<VertexBuffer>(Gfx, vertices));
		AddStaticBindable(std::make_unique<IndexBuffer>(Gfx, indices));
		AddStaticBindable(std::make_unique<InputLayout>(Gfx, vl.GetD3DLayout(), (IShader*)VS.get()));
		AddStaticBindable(std::move(VS));
		AddStaticBindable(std::make_unique<PixelShaderCommon>(Gfx, L"GeometryPS.cso"));
		AddStaticBindable(std::make_unique<Topology>(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}
	AddBindable(std::make_unique<TransformBuffer>(Gfx, *this));
}

const char* House::ToString() const noexcept
{
	return "House";
}

DirectX::XMMATRIX House::GetTransform() const noexcept
{
	return dx::XMMatrixTranslation(pos.x, pos.y, pos.z) * dx::XMMatrixScaling(10.f,10.f,10.f);
}
