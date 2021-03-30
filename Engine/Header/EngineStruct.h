#ifndef __ENGINE_STRUCT_H__
#define __ENGINE_STRUCT_H__

typedef struct tagStaticVertex
{
	D3DXVECTOR3	vPosition;
	D3DXVECTOR3	vNormal;
	D3DXVECTOR3 vTangent;
	D3DXVECTOR3 vBiNormal;
	D3DXVECTOR2	vUV;
	//Light UV
}STATICVERTEX,*LPSTATICVERTEX;
#endif // !__ENGINE_STRUCT_H__