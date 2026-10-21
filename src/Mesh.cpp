#include <Nova/Mesh.hpp>
#include <Nova/Renderer.hpp>
#include <Nova/ResourceManager.hpp>

#include <iostream>

extern Material* mat;

void* Mesh::mCB;
bool Mesh::frustum = true;

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

	//if (frustum)
	{
		Matrix MVP = Transform::projection * Transform::view * (parent * transform);
		Vector4 corners[8] =
		{
			MVP * Vector4{ aabb.min.x, aabb.min.y, aabb.min.z, 1.0f },
			MVP * Vector4{ aabb.max.x, aabb.min.y, aabb.min.z, 1.0f },
			MVP * Vector4{ aabb.min.x, aabb.max.y, aabb.min.z, 1.0f },
			MVP * Vector4{ aabb.min.x, aabb.min.y, aabb.max.z, 1.0f },
			MVP * Vector4{ aabb.max.x, aabb.max.y, aabb.max.z, 1.0f },
			MVP * Vector4{ aabb.min.x, aabb.max.y, aabb.max.z, 1.0f },
			MVP * Vector4{ aabb.max.x, aabb.min.y, aabb.max.z, 1.0f },
			MVP * Vector4{ aabb.max.x, aabb.max.y, aabb.min.z, 1.0f }
		};

		
		bool outLeft = true;
		for (auto& corner : corners) if (corner.x >= -corner.w) { outLeft = false; break; };
		if (outLeft)
			return;

		bool outRight = true;
		for (auto& corner : corners) if (corner.x <= corner.w) { outRight = false;  break; };
		if (outRight)
			return;

		bool outTop = true;
		for (auto& corner : corners) if (corner.y <= corner.w) { outTop = false;  break; };
		if (outTop)
			return;

		bool outBottom = true;
		for (auto& corner : corners) if (corner.y >= -corner.w) { outBottom = false;  break; };
		if (outBottom)
			return;

		bool outNear = true;
		for (auto& corner : corners) if (corner.z >= 0) { outNear = false;  break; };
		if (outNear)
			return;

		bool outFar = true;
		for (auto& corner : corners) if (corner.z <= corner.w) { outFar = false;  break; };
		if (outFar)
			return;

		//for (auto& corner : corners)
		//	if (corner.x >= -corner.w && corner.x <= corner.w && corner.y <= corner.w && corner.y >= -corner.w && corner.z >= 0 && corner.z <= corner.w)
		//		Renderer::DrawPoint({ corner.x / corner.w, corner.y / corner.w }, Transform::projection[1][1] / Transform::projection[0][0]);
	}

	Renderer::BindVertexBuffer(mVB, sizeof(Vertex), 0u);
	Renderer::BindIndexBuffer(mIB);
	Renderer::BindVSConstantBuffer(mCB, 0u);
	material->Bind();
	Renderer::DrawIndexed(mCount);
}
