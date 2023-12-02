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
		if (NodeMeshesPtrs.empty())
			return;

		if constexpr (std::is_same_v<BufferType, ObjBuffDiffNorm>)
		{
			if (ImGui::Begin("Node control window"))
			{
				BuildTree<ObjBuffDiffNorm>(Gfx, &*this);
			}
			ImGui::End();
		}
	}

	template<class T, typename BufferType = T>
	void BuildTree(Graphics& Gfx, Node* pNode)
	{
		if(!pNode)
			ImGui::TreePop();

		std::string NodeTag = std::string("Node ") + std::to_string(pNode->id) +  std::string(" control window");
		ImGui::TreeNode(NodeTag.c_str());

		for (size_t i = 0; i < pNode->NodeMeshesPtrs.size(); i++)
		{
			if (auto pCBuffer = NodeMeshesPtrs[i]->QueryBindable<PixelConstantBuffer<ObjBuffDiffNorm>>())
			{
				static ObjBuffDiffNorm Data = {};
				bool changed = false;

				changed |= ImGui::Checkbox("Normal map", &Data.isNormalMapEnabled);
				changed |= ImGui::Checkbox("Diffuse map", &Data.isDiffuseMapEnabled);
				changed |= ImGui::ColorPicker3("Normal map", &Data.diffuseColor.x);

				if (changed)
					pCBuffer->Update(Gfx, Data);
			}
		}
		

		for (size_t i = 0; i < pNode->pChildren.size(); i++)
		{
			BuildTree<T>(Gfx, pNode->pChildren[i].get());
		}
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