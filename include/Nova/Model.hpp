#pragma once

#include <Nova/Mesh.hpp>

class Model : public Node
{
public:
	Model(const char* path);

	void Draw();

private:
	MeshInstance** mMeshes = nullptr;
	unsigned int mNumMeshes = 0u;
};