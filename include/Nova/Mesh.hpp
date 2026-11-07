#pragma once

#include <Nova/Nova.hpp>
#include <Nova/Node.hpp>
#include <Nova/Material.hpp>

struct Vertex
{
	Vector3 position;
	Vector3 tanget;
	Vector3 biTanget;
	Vector3 normal;
	Vector2 texCoord;
};

class Mesh
{
public:
	static Mesh* Cube();
	static Mesh* Plane();

	struct AABB
	{
		Vector3 min;
		Vector3 max;
	} aabb;

private:
	Mesh(Vertex* vertices, size_t vSize, unsigned int* indices, size_t iSize);
	Mesh(void* vb, void* ib, size_t count);

	friend class ResourceManager;
	friend class MeshInstance;
	friend class Model;
	friend void Nova::Init();

	static void* mVS;
	static void* mPS;
	static void* mIL;
	static void* mCB;

	void* mVB;
	void* mIB;
	unsigned int mCount;
};

class MeshInstance : public Node
{
public:
	MeshInstance(Mesh*);
	void Draw(void* PS = Mesh::mPS);

	Material* material;

	Mesh* mesh;
};