#ifndef _RENDERINFORMATION_H_
#define _RENDERINFORMATION_H_
#include "EngineTypedef.h"
#include "EngineDefine.h"
#include "RenderProperty.h"
#include "FMath.hpp"

BEGIN(ENGINE)
class ENGINE_DLL RenderInformation
{
public:
	Vector4 CameraLocation{0,0,0,1};
	Matrix  ViewInverse{};
	Matrix  CameraView{}; 
	Matrix  CameraProjection{};
	Matrix  Ortho{};
};
END
#endif // !_RENDERINFORMATION_H_



