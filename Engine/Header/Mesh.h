#ifndef __MESH_H__
#define __MESH_H__
#include "Resource.h"
BEGIN(ENGINE)
class SubMesh;
class Texture;

typedef struct tagMaterial
{
	D3DMATERIAL9					tMaterial;
	std::shared_ptr<Texture>	pDiffuse;
	std::shared_ptr<Texture>	pSpecular;
	std::shared_ptr<Texture>	pNormal;
}MATERIAL, * LPMATERIAL;

class ENGINE_DLL Mesh abstract : public Resource
{
protected:
	explicit Mesh(LPDIRECT3DDEVICE9 const _pDevice);
	explicit Mesh(const Mesh& _rOther);
	virtual ~Mesh() = default;
	// Resource을(를) 통해 상속됨
	virtual void Free()			PURE;
public:
	// Resource을(를) 통해 상속됨
	virtual Resource* Clone()	PURE;
};
END
#endif // !__MESH_H__