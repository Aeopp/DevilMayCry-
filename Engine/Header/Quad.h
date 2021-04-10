#ifndef _QUAD_H_
#define _QUAD_H_
#include "EngineTypedef.h"
#include "EngineDefine.h"

BEGIN(ENGINE)
struct ENGINE_DLL Quad
{
	void Initialize(LPDIRECT3DDEVICE9 _Device)&;
	
	void Render(LPDIRECT3DDEVICE9 _Device,
				/*������� Ŭ���̾�Ʈ â ����� ���� ���� .. */
				const float SizeXRatio,
				const float SizeYRatio,
				LPD3DXEFFECT const Fx)&;
	void Release()&;

	~Quad()noexcept { Release();  };

	LPDIRECT3DVERTEXBUFFER9 VtxBuf{ nullptr };
	LPDIRECT3DINDEXBUFFER9 IdxBuf{ nullptr };
	LPDIRECT3DVERTEXDECLARATION9 VtxDecl{ nullptr };
	uint64 Stride{ 0u };
};
END


#endif // !


