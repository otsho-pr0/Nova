#include <Nova/Mesh.hpp>
#include <Nova/Renderer.hpp>
#include <Nova/ResourceManager.hpp>

#include <iostream>

extern Material* mat;

void* Mesh::mCB;

Mesh::Mesh(Vertex* vertices, size_t vSize, unsigned int* indices, size_t iSize) :
	material(mat)
{
	mVB = Renderer::CreateVertexBuffer(vertices, vSize);
	mIB = Renderer::CreateIndexBuffer(indices, iSize);
	mCount = iSize / sizeof(unsigned int);
}

Mesh::Mesh(void* vb, void* ib, size_t count) :
	material(mat)
{
	mVB = vb;
	mIB = ib;
	mCount = count;
}

void Mesh::DrawWithNoDraw(Matrix parent)
{
	struct
	{
		Matrix projection;
		Matrix view;
		Matrix model;
	} cbuffer = { Transform::projection, Transform::view, parent * transform };
	Renderer::UpdateConstantBuffer(mCB, &cbuffer, sizeof(cbuffer));
	Renderer::BindVertexBuffer(mVB, sizeof(Vertex), 0u);
	Renderer::BindIndexBuffer(mIB);
	Renderer::BindVSConstantBuffer(mCB, 0u);
	material->Bind();
	Renderer::DrawIndexed(mCount);
}

Mesh* Mesh::Cube()
{
	Vertex vertices[] =
	{
		{ { -0.5f, -0.5f, -0.5f }, {  1.0f,  0.0f,  0.0f }, {  0.0f, -1.0f,  0.0f }, {  0.0f,  0.0f, -1.0f }, { 0.0f, 1.0f } },
		{ { -0.5f,  0.5f, -0.5f }, {  1.0f,  0.0f,  0.0f }, {  0.0f, -1.0f,  0.0f }, {  0.0f,  0.0f, -1.0f }, { 0.0f, 0.0f } },
		{ {  0.5f, -0.5f, -0.5f }, {  1.0f,  0.0f,  0.0f }, {  0.0f, -1.0f,  0.0f }, {  0.0f,  0.0f, -1.0f }, { 1.0f, 1.0f } },
		{ {  0.5f,  0.5f, -0.5f }, {  1.0f,  0.0f,  0.0f }, {  0.0f, -1.0f,  0.0f }, {  0.0f,  0.0f, -1.0f }, { 1.0f, 0.0f } },

		{ { -0.5f, -0.5f,  0.5f }, {  1.0f,  0.0f,  0.0f }, {  0.0f, -1.0f,  0.0f }, {  0.0f,  0.0f,  1.0f }, { 0.0f, 1.0f } },
		{ { -0.5f,  0.5f,  0.5f }, {  1.0f,  0.0f,  0.0f }, {  0.0f, -1.0f,  0.0f }, {  0.0f,  0.0f,  1.0f }, { 0.0f, 0.0f } },
		{ {  0.5f, -0.5f,  0.5f }, {  1.0f,  0.0f,  0.0f }, {  0.0f, -1.0f,  0.0f }, {  0.0f,  0.0f,  1.0f }, { 1.0f, 1.0f } },
		{ {  0.5f,  0.5f,  0.5f }, {  1.0f,  0.0f,  0.0f }, {  0.0f, -1.0f,  0.0f }, {  0.0f,  0.0f,  1.0f }, { 1.0f, 0.0f } },

		{ { -0.5f, -0.5f, -0.5f }, {  1.0f,  0.0f,  0.0f }, {  0.0f,  0.0f, -1.0f }, {  0.0f, -1.0f,  0.0f }, { 0.0f, 1.0f } },
		{ { -0.5f, -0.5f,  0.5f }, {  1.0f,  0.0f,  0.0f }, {  0.0f,  0.0f, -1.0f }, {  0.0f, -1.0f,  0.0f }, { 0.0f, 0.0f } },
		{ {  0.5f, -0.5f, -0.5f }, {  1.0f,  0.0f,  0.0f }, {  0.0f,  0.0f, -1.0f }, {  0.0f, -1.0f,  0.0f }, { 1.0f, 1.0f } },
		{ {  0.5f, -0.5f,  0.5f }, {  1.0f,  0.0f,  0.0f }, {  0.0f,  0.0f, -1.0f }, {  0.0f, -1.0f,  0.0f }, { 1.0f, 0.0f } },

		{ { -0.5f,  0.5f, -0.5f }, {  1.0f,  0.0f,  0.0f }, {  0.0f,  0.0f, -1.0f }, {  0.0f,  1.0f,  0.0f }, { 0.0f, 1.0f } },
		{ { -0.5f,  0.5f,  0.5f }, {  1.0f,  0.0f,  0.0f }, {  0.0f,  0.0f, -1.0f }, {  0.0f,  1.0f,  0.0f }, { 0.0f, 0.0f } },
		{ {  0.5f,  0.5f, -0.5f }, {  1.0f,  0.0f,  0.0f }, {  0.0f,  0.0f, -1.0f }, {  0.0f,  1.0f,  0.0f }, { 1.0f, 1.0f } },
		{ {  0.5f,  0.5f,  0.5f }, {  1.0f,  0.0f,  0.0f }, {  0.0f,  0.0f, -1.0f }, {  0.0f,  1.0f,  0.0f }, { 1.0f, 0.0f } },

		{ { -0.5f, -0.5f, -0.5f }, {  0.0f,  1.0f,  0.0f }, {  0.0f,  0.0f, -1.0f }, { -1.0f,  0.0f,  0.0f }, { 0.0f, 1.0f } },
		{ { -0.5f, -0.5f,  0.5f }, {  0.0f,  1.0f,  0.0f }, {  0.0f,  0.0f, -1.0f }, { -1.0f,  0.0f,  0.0f }, { 0.0f, 0.0f } },
		{ { -0.5f,  0.5f, -0.5f }, {  0.0f,  1.0f,  0.0f }, {  0.0f,  0.0f, -1.0f }, { -1.0f,  0.0f,  0.0f }, { 1.0f, 1.0f } },
		{ { -0.5f,  0.5f,  0.5f }, {  0.0f,  1.0f,  0.0f }, {  0.0f,  0.0f, -1.0f }, { -1.0f,  0.0f,  0.0f }, { 1.0f, 0.0f } },

		{ {  0.5f, -0.5f, -0.5f }, {  0.0f,  1.0f,  0.0f }, {  0.0f,  0.0f, -1.0f }, {  1.0f,  0.0f,  0.0f }, { 0.0f, 1.0f } },
		{ {  0.5f, -0.5f,  0.5f }, {  0.0f,  1.0f,  0.0f }, {  0.0f,  0.0f, -1.0f }, {  1.0f,  0.0f,  0.0f }, { 0.0f, 0.0f } },
		{ {  0.5f,  0.5f, -0.5f }, {  0.0f,  1.0f,  0.0f }, {  0.0f,  0.0f, -1.0f }, {  1.0f,  0.0f,  0.0f }, { 1.0f, 1.0f } },
		{ {  0.5f,  0.5f,  0.5f }, {  0.0f,  1.0f,  0.0f }, {  0.0f,  0.0f, -1.0f }, {  1.0f,  0.0f,  0.0f }, { 1.0f, 0.0f } }
	};

	unsigned int indices[] =
	{
		 0,  1,  2,  2,  1,  3,
		 4,  6,  5,  6,  7,  5,
		 8, 10,  9, 10, 11,  9,
		12, 13, 14, 14, 13, 15,
		16, 17, 18, 18, 17, 19,
		20, 22, 21, 22, 23, 21
	};

	static void* vb = Renderer::CreateVertexBuffer(vertices, sizeof(vertices));
	static void* ib = Renderer::CreateIndexBuffer(indices, sizeof(indices));

	return ResourceManager::CreateMesh("shemt", vb, ib, sizeof(indices) / sizeof(*indices));
}

Mesh* Mesh::Plane()
{
	Vertex vertices[] =
	{
		{ { -0.5f,  0.0f, -0.5f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f } },
		{ { -0.5f,  0.0f,  0.5f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f } },
		{ {  0.5f,  0.0f, -0.5f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 1.0f } },
		{ {  0.5f,  0.0f,  0.5f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f } },
	};

	unsigned int indices[] =
	{
		 0,  1,  2,  2,  1,  3
	};

	static void* vb = Renderer::CreateVertexBuffer(vertices, sizeof(vertices));
	static void* ib = Renderer::CreateIndexBuffer(indices, sizeof(indices));

	AABB aabb = { { -0.5f,  0.0f, -0.5f }, {  0.5f,  0.0f,  0.5f } };

	Mesh* r = ResourceManager::CreateMesh("shmet", vb, ib, sizeof(indices) / sizeof(*indices));
	r->aabb = aabb;
	return r;
}

void Mesh::Draw(Matrix parent)
{
	struct
	{
		Matrix projection;
		Matrix view;
		Matrix model;
	} cbuffer = { Transform::projection, Transform::view, parent * transform };
	Renderer::UpdateConstantBuffer(mCB, &cbuffer, sizeof(cbuffer));

	/*
		Left: row 4 + row 1
		Right: row 4 - row 1
		Bottom: row 4 + row 2
		Top: row 4 - row 2
		near: row 3
		far: row 4 - row 3
	*/

	Matrix camera = Transform::projection * Transform::view;

	Vector3 center = cbuffer.model * ((aabb.max + aabb.min) / 2.0f);
	Vector3 half_extents = cbuffer.model * ((aabb.max - aabb.min) / 2.0f);

	Vector4 left   = camera[3] + camera[0];
	Vector4 right  = camera[3] - camera[0];
	Vector4 bottom = camera[3] + camera[1];
	Vector4 top    = camera[3] - camera[1];
	Vector4 near   = camera[2];
	Vector4 far    = camera[2] - camera[2];

	float s = Vector3::Dot(left, center) + left.w;
	float r = Vector3::Dot({ Math::Abs(left.x), Math::Abs(left.y), Math::Abs(left.z) }, half_extents);
	if (s + r < 0.0f)
		return;

	s = Vector3::Dot(right, center) + right.w;
	r = Vector3::Dot({ Math::Abs(right.x), Math::Abs(right.y), Math::Abs(right.z) }, half_extents);
	if (s + r < 0.0f)
		return;

	s = Vector3::Dot(bottom, center) + bottom.w;
	r = Vector3::Dot({ Math::Abs(bottom.x), Math::Abs(bottom.y), Math::Abs(bottom.z) }, half_extents);
	if (s + r < 0.0f)
		return;

	s = Vector3::Dot(top, center) + top.w;
	r = Vector3::Dot({ Math::Abs(top.x), Math::Abs(top.y), Math::Abs(top.z) }, half_extents);
	if (s + r < 0.0f)
		return;

	s = Vector3::Dot(near, center) + near.w;
	r = Vector3::Dot({ Math::Abs(near.x), Math::Abs(near.y), Math::Abs(near.z) }, half_extents);
	if (s + r < 0.0f)
		return;

	s = Vector3::Dot(far, center) + far.w;
	r = Vector3::Dot({ Math::Abs(far.x), Math::Abs(far.y), Math::Abs(far.z) }, half_extents);
	if (s + r < 0.0f)
		return;

	Renderer::BindVertexBuffer(mVB, sizeof(Vertex), 0u);
	Renderer::BindIndexBuffer(mIB);
	Renderer::BindVSConstantBuffer(mCB, 0u);
	material->Bind();
	Renderer::DrawIndexed(mCount);
}
