#ifndef __MATERIAL_H__
#define __MATERIAL_H__
#include "assimp/material.h"
#include "EngineDefine.h"
#include "Texture.h"
#include "EngineTypedef.h"
#include <string>
#include <unordered_map>
#include <memory>
#include <vector>

BEGIN(ENGINE)
struct Material
{
	std::string Name{};
	// 텍스쳐 타입 -> 텍스쳐 인덱스 . 
	std::unordered_map<aiTextureType,
		std::vector<std::shared_ptr<Texture>>> Textures{};
};

END

#endif // !__MATERIAL_H__