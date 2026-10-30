#include <Nova/Node.hpp>

void Node::Update(Matrix parent)
{
	transform.Update();
	mParent = parent;
	for (auto& child : children)
		child->Update(parent * transform);
}