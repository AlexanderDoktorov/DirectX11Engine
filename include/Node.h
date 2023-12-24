#pragma once
#include <vector>
#include <memory>
#include <string>
#include <type_traits>
#include "Mesh.h"

class Node
{
	friend class Model;
public:
	Node(int id, const std::string& nodeName, std::vector<Mesh*> meshesPtrs, const DirectX::XMMATRIX& mTransormation) 
		: 
		id(id), 
		nodeTag(nodeName + "_" + std::to_string(id)),
		NodeMeshesPtrs(std::move(meshesPtrs))
	{
		dx::XMStoreFloat4x4(&nodeTransformation, mTransormation);
		dx::XMStoreFloat4x4(&nodeAppliedTransformation, dx::XMMatrixIdentity());
	}

	void Draw(Graphics& Gfx, dx::FXMMATRIX accumulatedTransform) const
	{
		const dx::XMMATRIX rusultiveTransoform =
			dx::XMLoadFloat4x4( &nodeAppliedTransformation ) *
			dx::XMLoadFloat4x4( &nodeTransformation ) * 
			accumulatedTransform;
		for( const Mesh* pMesh : NodeMeshesPtrs )
		{
			pMesh->Draw(Gfx, rusultiveTransoform );
		}
		for( const auto& pChild : childrenPtrs)
		{
			pChild->Draw(Gfx, rusultiveTransoform);
		}
	}

	Node& SetAppliedTransform( DirectX::FXMMATRIX transformMatrix ) noexcept
	{
		dx::XMStoreFloat4x4( &nodeAppliedTransformation,transformMatrix );
		return *this;
	}

	void Clear() noexcept
	{
		id = -1;
		nodeTag.clear();
		NodeMeshesPtrs.clear();
		for (auto& pChild : childrenPtrs)
		{
			pChild->Clear();
		}
	}

protected:
	void Reserve(size_t cap);
	void AddChild(std::unique_ptr<Node> pNode) noxnd;
private:
	int id;
	std::string nodeTag;
	dx::XMFLOAT4X4 nodeTransformation;
	dx::XMFLOAT4X4 nodeAppliedTransformation;
	std::vector<Mesh*> NodeMeshesPtrs;
	std::vector<std::unique_ptr<Node>> childrenPtrs;
};