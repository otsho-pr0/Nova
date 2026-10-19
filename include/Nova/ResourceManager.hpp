#pragma once

#include <Nova/Mesh.hpp>
#include <Nova/Material.hpp>

#include <unordered_map>
#include <initializer_list>
#include <string>

class ResourceManager
{
public:
	static Material* CreateMaterial(std::string name, const char* diffuse, const char* normals, const char* specular);
	static Material* GetMaterial(std::string name);
	static bool HasMaterial(std::string name);
	static auto MaterialsBegin() { return materials.begin(); }
	static auto MaterialsEnd() { return materials.end(); }

	static Mesh* CreateMesh(std::string name, Vertex* vertices, size_t vSize, unsigned int* indices, size_t iSize);
	static Mesh* CreateMesh(std::string name, void* vb, void* ib, size_t count);
	static Mesh* GetMesh(std::string name);
	static bool HasMesh(std::string name);
	static auto MeshesBegin() { return meshes.begin(); }
	static auto MeshesEnd() { return meshes.end(); }

private:
	static std::unordered_map<std::string, Mesh*> meshes;
	static std::unordered_map<std::string, Material*> materials;
};