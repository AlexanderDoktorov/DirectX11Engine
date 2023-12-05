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

	template<class T, typename BufferType = T>
	void ShowContolWindow(Graphics& Gfx)
	{
		// TO DO
	}

	template<class T, typename BufferType = T>
	void BuildTree(Graphics& Gfx, Node* pNode)
	{
		// TO DO
	}

private:
	void Reserve(size_t cap);
	void AddChild(std::unique_ptr<Node> pNode) noexcept(!_DEBUG);
private:
	int id;
	std::string nodeTag;
	std::vector<Mesh*> NodeMeshesPtrs;
	std::vector<std::unique_ptr<Node>> pChildren;
};