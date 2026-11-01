#pragma once

class Material
{
public:
	void* GetDiffuse();
	void* GetNormals();
	void* GetEmissive();
	void Bind();

private:
	Material(const char* diffuse, const char* normals, const char* emissive);
	~Material();

	friend class ResourceManager;

	void* mDiffuse = nullptr;
	void* mNormals = nullptr;
	void* mRoughness = nullptr;
	void* mMetalness = nullptr;
	void* mEmissive = nullptr;
};