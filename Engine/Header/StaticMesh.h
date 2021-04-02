#ifndef __STATIC_MESH_H__
#define __STATIC_MESH_H__
#include "Mesh.h"
#include "RenderInterface.h"
BEGIN(ENGINE)
class ENGINE_DLL StaticMesh final : public Mesh
									
{
private:
	explicit StaticMesh(LPDIRECT3DDEVICE9 const _pDevice);
	explicit StaticMesh(const StaticMesh& _rOther);
	virtual ~StaticMesh() = default;
	// Mesh을(를) 통해 상속됨
	virtual void Free() override;
public:
	static StaticMesh* Create(LPDIRECT3DDEVICE9 const _pDevice, 
							const std::filesystem::path _Path);
	// Mesh을(를) 통해 상속됨
	virtual Resource* Clone() override;
public:
	HRESULT LoadMeshFromFile(const std::filesystem::path _Path)&;
};
END
#endif // !__STATIC_MESH_H__