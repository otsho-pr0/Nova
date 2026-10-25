#include <Nova/Node.hpp>

void Node::Draw(Matrix parent)
{
	for (auto& child : children)
		child->Draw(parent * transform);
}