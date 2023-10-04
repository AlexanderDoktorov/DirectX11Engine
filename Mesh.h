#pragma once
#include "IndexedTriangleList.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/Scene.h"

class Mesh
{
public:
	Mesh(std::string objFile, DynamicVertex::VertexLayout vl,  unsigned int aippFlags = aiPostProcessSteps::aiProcess_Triangulate)
	{
		Assimp::Importer importer;
		auto pScene = importer.ReadFile(objFile, aippFlags);

		size_t totalVertices = 0;
		size_t totalFaces = 0;
		for (size_t i = 0; i < pScene->mNumMeshes; i++)
		{
			totalVertices += pScene->mMeshes[i]->mNumVertices;
			totalFaces += pScene->mMeshes[i]->mNumFaces;
		}
		DynamicVertex::VertexBuffer vb(vl, totalVertices);
		std::vector<unsigned short> indices;
		indices.reserve(totalFaces * 3);

		size_t verticesPassed = 0;
		for (size_t meshIdx = 0; meshIdx < pScene->mNumMeshes; meshIdx++)
		{
			const auto pMesh = pScene->mMeshes[meshIdx];

			for (size_t i = verticesPassed, j = 0; i < pMesh->mNumVertices + verticesPassed; i++, j++)
			{
				if (vl.HasSuchElement<DynamicVertex::VertexLayout::Position3D>())
					vb[i].Attr<DynamicVertex::VertexLayout::Position3D>() = *(dx::XMFLOAT3*)(&pMesh->mVertices[j]);
				if (vl.HasSuchElement<DynamicVertex::VertexLayout::Normal>())
					vb[i].Attr<DynamicVertex::VertexLayout::Normal>() = *(dx::XMFLOAT3*)(&pMesh->mNormals[j]);
			}
			for (size_t i = 0; i < pMesh->mNumFaces; i++)
			{
				const auto& face = pMesh->mFaces[i];
				assert(face.mNumIndices == 3);
				indices.push_back(static_cast<unsigned short>(face.mIndices[0] + verticesPassed));
				indices.push_back(static_cast<unsigned short>(face.mIndices[1] + verticesPassed));
				indices.push_back(static_cast<unsigned short>(face.mIndices[2] + verticesPassed));
			}
			verticesPassed += pMesh->mNumVertices;
		}
		pItl = std::make_unique<IndexedTriangleList>(vb, indices);
	}

	std::unique_ptr<IndexedTriangleList> pItl;
};