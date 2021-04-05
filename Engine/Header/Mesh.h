#ifndef __MESH_H__
#define __MESH_H__
#include "Resource.h"
BEGIN(ENGINE)
class Subset;
class ENGINE_DLL Mesh abstract : public Resource
{
protected:
	std::vector<std::shared_ptr<Subset>>  m_vecSubset;
protected:
	explicit Mesh(LPDIRECT3DDEVICE9 const _pDevice);
	explicit Mesh(const Mesh& _rOther);
	virtual ~Mesh() = default;
	// Resource을(를) 통해 상속됨
	virtual void Free()			PURE;
public:
	// Resource을(를) 통해 상속됨
	virtual Resource* Clone()	PURE;
public:
	HRESULT Render();
public:
	const uint64			GetNumSubset() const; 
	std::weak_ptr<Subset>	GetSubset(const UINT _nIndex);
	const VERTEXBUFFERDESC& GetVertexBufferDesc(const UINT _nSubsetIdx);
	const MATERIAL&			GetMaterial(const UINT _nSubsetIdx);
};

END
#endif // !__MESH_H__