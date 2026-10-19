#pragma once

class Material
{
public:
	void* GetDiffuse();
	void* GetNormals();
	void* GetSpecular();
	void Bind();

private:
	Material(const char* diffuse, const char* normals, const char* specular);
	~Material();

	friend class ResourceManager;

	void* mDiffuse = nullptr;
	void* mNormals = nullptr;
	void* mSpecular = nullptr;
};