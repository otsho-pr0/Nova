#pragma once

#include <Nova/Mesh.hpp>

class Model : public Node
{
public:
	Model(const char* path);
	~Model();

private:
	Mesh** mMeshes;
	unsigned int mNumMeshes;
};