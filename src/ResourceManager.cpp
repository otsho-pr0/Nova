#include <Nova/ResourceManager.hpp>

std::unordered_map<std::string, Mesh*> ResourceManager::meshes;
std::unordered_map<std::string, Material*> ResourceManager::materials;

Material* ResourceManager::CreateMaterial(std::string name, const char* diffuse, const char* normals, const char* specular)
{
	return materials[name] = new Material(diffuse, normals, specular);
}

Material* ResourceManager::GetMaterial(std::string name)
{
	return materials.at(name);
}

bool ResourceManager::HasMaterial(std::string name)
{
	return materials.contains(name);
}

Mesh* ResourceManager::CreateMesh(std::string name, Vertex* vertices, size_t vSize, unsigned int* indices, size_t iSize)
{
	return meshes[name] = new Mesh(vertices, vSize, indices, iSize);
}

Mesh* ResourceManager::CreateMesh(std::string name, void* vb, void* ib, size_t count)
{
	return meshes[name] = new Mesh(vb, ib, count);
}

Mesh* ResourceManager::GetMesh(std::string name)
{
	return meshes.at(name);
}

bool ResourceManager::HasMesh(std::string name)
{
	return meshes.contains(name);
}
