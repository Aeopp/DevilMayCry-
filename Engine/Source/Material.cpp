#include "Material.h"
#include "assimp/material.h"
#include "Texture.h"

USING(ENGINE)

Material::Material()
	: Name("")
{
}

void Material::Free()
{
	for (auto rPair : Textures)
	{
		rPair.second.clear();
		rPair.second.shrink_to_fit();
	}
	Textures.clear();
	Object::Free();
}

Material* Material::Create()
{
	Material* pInstance = new Material;
	return pInstance;
}
