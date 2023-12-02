#include "Node.h"

void Node::Reserve(size_t cap)
{
	pChildren.reserve(cap);
}

void Node::AddChild(std::unique_ptr<Node> pNode) noexcept(!_DEBUG)
{
	assert(pNode != nullptr);
	pChildren.push_back(std::move(pNode));
}
