#include "Node.h"

void Node::Reserve(size_t cap)
{
	childrenPtrs.reserve(cap);
}

void Node::AddChild(std::unique_ptr<Node> pNode) noxnd
{
	assert(pNode != nullptr);
	childrenPtrs.push_back(std::move(pNode));
}
