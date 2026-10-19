#pragma once

#include <Nova/Mesh.hpp>

class Model
{
public:
	Model(const char* path);
	~Model();

	void Draw();

	Transform transform;

private:
	Mesh** mMeshes;
	unsigned int mNumMeshes;
};