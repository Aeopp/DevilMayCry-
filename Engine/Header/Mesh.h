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
	// Resource��(��) ���� ��ӵ�
	virtual void Free()			PURE;
public:
	// Resource��(��) ���� ��ӵ�
	virtual Resource* Clone()	PURE;
private:

};

END
#endif // !__MESH_H__