#include "Subset.h"

USING(ENGINE)
Subset::Subset(LPDIRECT3DDEVICE9 const _pDevice)
	: _Device(_pDevice)
{
	SafeAddRef(_pDevice);
}

void Subset::Free()
{
	_VertexInformation.Release();
	SafeRelease(_Device);
	
	Object::Free();
}

Subset* Subset::Create(LPDIRECT3DDEVICE9 const _pDevice)
{
	Subset* _Ptr = new Subset(_pDevice);
	return _Ptr;
}

void Subset::Initialize(
	const VertexInformation& InitVertexInformation, 
	const Material&          InitMaterial)
{
	_VertexInformation = InitVertexInformation;
	_Material          = InitMaterial; 
}
