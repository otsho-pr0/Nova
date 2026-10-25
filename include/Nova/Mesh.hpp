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

class Mesh : public Node
{
public:
	static Mesh* Cube();
	static Mesh* Plane();

	void Draw(Matrix parent = Matrix(1.0f)) override;

	struct AABB
	{
		Vector3 min;
		Vector3 max;
	} aabb;

	Material* material;

	static bool frustum;

private:
	Mesh(Vertex* vertices, size_t vSize, unsigned int* indices, size_t iSize);
	Mesh(void* vb, void* ib, size_t count);

	friend class ResourceManager;
	friend void Nova::Init();

	void* mVB;
	void* mIB;
	static void* mCB;
	unsigned int mCount;
};