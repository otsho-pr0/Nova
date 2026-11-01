#include <Nova/Material.hpp>
#include <Nova/Renderer.hpp>
#include <Nova/ResourceManager.hpp>

#include <stb_image.h>

Material::Material(const char* diffuse, const char* normals, const char* emisive)
{
	int width, height, channels;
	unsigned char* data = stbi_load(diffuse, &width, &height, &channels, 4);
	mDiffuse = Renderer::CreateTextureWithMipmapping(data, width, height, channels, true);
	stbi_image_free(data);
	data = stbi_load(normals, &width, &height, &channels, 4);
	mNormals = Renderer::CreateTextureWithMipmapping(data, width, height, channels);
	stbi_image_free(data);
	data = stbi_load(emisive, &width, &height, &channels, 4);
	mEmissive = Renderer::CreateTextureWithMipmapping(data, width, height, channels, true);
	stbi_image_free(data);
}

Material::~Material()
{
	if (mDiffuse)
		Renderer::DestroyTexture(mDiffuse);
}

void* Material::GetDiffuse()
{
	return mDiffuse;
}

void* Material::GetNormals()
{
	return mNormals;
}

void* Material::GetEmissive()
{
	return mEmissive;
}

void Material::Bind()
{
	Renderer::BindTexture(mDiffuse, 0u);
	Renderer::BindTexture(mNormals, 1u);
	Renderer::BindTexture(mEmissive, 2u);
}
