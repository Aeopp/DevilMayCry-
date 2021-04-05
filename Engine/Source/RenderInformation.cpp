#include "RenderInformation.h"
USING(ENGINE)

void RenderInformation::Make(const Matrix& _CameraView, const Matrix& _CameraProjection)
{
	CameraView = _CameraView;
	CameraProjection = _CameraProjection;
	ViewInverse = FMath::Inverse(CameraView);
}
