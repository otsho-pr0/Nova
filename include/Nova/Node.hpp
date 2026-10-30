#pragma once

#include <Nova/Transform.hpp>
#include <vector>
#include <string>

class Node
{
public:
	Transform transform;
	std::string name;

	virtual void Update(Matrix parent = Matrix(1.0f));

	Node* parent = nullptr;
	std::vector<Node*> children;

protected:
	Matrix mParent;
};