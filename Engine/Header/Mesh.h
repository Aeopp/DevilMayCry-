#ifndef __MESH_H__
#define __MESH_H__
#include "Resource.h"
#include "Subset.h"
#include "EngineTypedef.h"
BEGIN(ENGINE)
class Texture;

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
	auto GetLocalVertexLocations() const& { return LocalVertexLocations; };
protected:
	std::shared_ptr<std::vector<Vector3>> LocalVertexLocations{};
	std::shared_ptr<std::vector<Subset>>  SubSets{};
};
END
#endif // !__MESH_H__