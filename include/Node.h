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
	Node(int id, std::string nodeName, const std::vector<Mesh*>& NodeMeshesPtrs) : 
		NodeMeshesPtrs(NodeMeshesPtrs), 
		id(id), 
		nodeTag(nodeName + "_" + std::to_string(id))
	{

	}

	void Clear() noexcept
	{
		id = -1;
		nodeTag.clear();
		NodeMeshesPtrs.clear();
		for (auto& pChild : pChildren)
		{
			pChild->Clear();
		}
	}

private:
	void Reserve(size_t cap);
	void AddChild(std::unique_ptr<Node> pNode) noxnd;
private:
	int id;
	std::string nodeTag;
	std::vector<Mesh*> NodeMeshesPtrs;
	std::vector<std::unique_ptr<Node>> pChildren;
};