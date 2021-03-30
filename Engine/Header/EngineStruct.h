#ifndef __ENGINE_STRUCT_H__
#define __ENGINE_STRUCT_H__

typedef struct tagStaticVertex
{
	D3DXVECTOR3	vPosition;
	D3DXVECTOR3	vNormal;
	D3DXVECTOR3 vTangent;
	D3DXVECTOR3 vBiNormal;
	D3DXVECTOR2	vUV;
}STATICVERTEX,*LPSTATICVERTEX;

typedef struct tagMaterial
{
	D3DMATERIAL9					tMaterial;
	std::shared_ptr<class Texture>	pDiffuse;
	std::shared_ptr<class Texture>	pSpecular;
	std::shared_ptr<class Texture>	pNormal;
}MATERIAL, *LPMATERIAL;
#endif // !__ENGINE_STRUCT_H__