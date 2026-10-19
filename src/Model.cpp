#include <Nova/Model.hpp>
#include <Nova/Mesh.hpp>
#include <Nova/ResourceManager.hpp>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <filesystem>

void ProcessNodes(Mesh** meshes, aiNode* parent, aiMatrix4x4 parentTransform = aiMatrix4x4())
{
	aiMatrix4x4 currentTransform = parentTransform * parent->mTransformation;

	aiVector3D position, rotation, scale;
	currentTransform.Decompose(scale, rotation, position);

	for (unsigned int m = 0u; m < parent->mNumMeshes; m++)
		meshes[parent->mMeshes[m]]->transform = { { position.x, position.y, position.z }, { rotation.x, rotation.y, rotation.z }, { scale.x, scale.y, scale.z } };

	for (unsigned int c = 0u; c < parent->mNumChildren; c++)
		ProcessNodes(meshes, parent->mChildren[c], currentTransform);
}

Model::Model(const char* path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipWindingOrder | aiProcess_MakeLeftHanded | aiProcess_FlipUVs | aiProcess_GenBoundingBoxes | aiProcess_RemoveRedundantMaterials | aiProcess_CalcTangentSpace);
	std::filesystem::path current = std::filesystem::path(path).parent_path();
	mMeshes = new Mesh*[scene->mNumMeshes];
	mNumMeshes = scene->mNumMeshes;

	std::unordered_map<unsigned int, std::string> materials;

	for (unsigned int m = 0u; m < mNumMeshes; m++)
	{
		const aiMesh* mesh = scene->mMeshes[m];
		Vertex* vertices = new Vertex[mesh->mNumVertices];
		for (unsigned int v = 0u; v < mesh->mNumVertices; v++)
		{
			vertices[v].position = { mesh->mVertices[v].x, mesh->mVertices[v].y, mesh->mVertices[v].z };
			vertices[v].tanget = { mesh->mTangents[v].x, mesh->mTangents[v].y, mesh->mTangents[v].z };
			vertices[v].biTanget = { mesh->mBitangents[v].x, mesh->mBitangents[v].y, mesh->mBitangents[v].z };
			vertices[v].normal = { mesh->mNormals[v].x, mesh->mNormals[v].y, mesh->mNormals[v].z };
			vertices[v].texCoord = { mesh->mTextureCoords[0][v].x, mesh->mTextureCoords[0][v].y };
		}

		unsigned int* indices = new unsigned int[mesh->mNumFaces * 3];
		for (unsigned int i = 0u; i < mesh->mNumFaces; i++)
		{
			indices[i * 3 + 0] = mesh->mFaces[i].mIndices[0];
			indices[i * 3 + 1] = mesh->mFaces[i].mIndices[1];
			indices[i * 3 + 2] = mesh->mFaces[i].mIndices[2];
		}

		if (ResourceManager::HasMesh(mesh->mName.C_Str()))
			mMeshes[m] = ResourceManager::GetMesh(mesh->mName.C_Str());
		else
			mMeshes[m] = ResourceManager::CreateMesh(mesh->mName.C_Str(), vertices, sizeof(Vertex) * mesh->mNumVertices, indices, sizeof(unsigned int) * mesh->mNumFaces * 3);

		aiString diffuse;
		scene->mMaterials[mesh->mMaterialIndex]->GetTexture(aiTextureType_DIFFUSE, 0, &diffuse);
		aiString normals;
		scene->mMaterials[mesh->mMaterialIndex]->GetTexture(aiTextureType_NORMALS, 0, &normals);
		aiString specular;
		scene->mMaterials[mesh->mMaterialIndex]->GetTexture(aiTextureType_METALNESS, 0, &specular);
		
		if (materials.contains(mesh->mMaterialIndex))
			mMeshes[m]->material = ResourceManager::GetMaterial(materials[mesh->mMaterialIndex].c_str());
		else
		{
			materials[mesh->mMaterialIndex] = std::filesystem::path(path).filename().string().append("_").append(std::to_string(mesh->mMaterialIndex));
			mMeshes[m]->material = ResourceManager::CreateMaterial(materials[mesh->mMaterialIndex], (current / diffuse.C_Str()).string().c_str(), (current / normals.C_Str()).string().c_str(), (current / specular.C_Str()).string().c_str());
		}

		mMeshes[m]->aabb.min = { mesh->mAABB.mMin.x, mesh->mAABB.mMin.y, mesh->mAABB.mMin.z };
		mMeshes[m]->aabb.max = { mesh->mAABB.mMax.x, mesh->mAABB.mMax.y, mesh->mAABB.mMax.z };

		delete[] vertices;
		delete[] indices;
	}

	ProcessNodes(mMeshes, scene->mRootNode);
}

Model::~Model()
{
	delete[] mMeshes;
}

void Model::Draw()
{
	for (unsigned int m = 0u; m < mNumMeshes; m++)
		mMeshes[m]->Draw(transform);
}