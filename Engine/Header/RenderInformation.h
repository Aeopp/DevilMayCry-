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
	void Make(const Matrix& _CameraView,const Matrix& _CameraProjection);
	Matrix  ViewInverse{};
	Matrix  CameraView{}; 
	Matrix  CameraProjection{};
};
END
#endif // !_RENDERINFORMATION_H_



