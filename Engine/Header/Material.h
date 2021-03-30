#ifndef __MATERIAL_H__
#define __MATERIAL_H__
#include "EngineDefine.h"
#include "Texture.h"

BEGIN(ENGINE)
struct Material 
{
	std::unordered_map<std::string, std::shared_ptr<Texture>> Textures{};
};
END
#endif // !__MATERIAL_H__