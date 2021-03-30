#ifndef __DYNAMIC_MESH_H__
#define __DYNAMIC_MESH_H__
#include "Mesh.h"
BEGIN(ENGINE)
class ENGINE_DLL DynamicMesh final : public Mesh
{
private:
	explicit DynamicMesh(LPDIRECT3DDEVICE9 const _pDevice);
	explicit DynamicMesh(const DynamicMesh& _rOther);
	virtual	~DynamicMesh() = default;
	// Mesh을(를) 통해 상속됨
	virtual void Free() override;
public:
	static DynamicMesh* Create(LPDIRECT3DDEVICE9 const _pDevice, const TSTRING& _sFilePath);
	// Mesh을(를) 통해 상속됨
	virtual Resource* Clone() override;
private:
	HRESULT LoadMeshFromFile(const std::filesystem::path _Path);
};
END
#endif // !__DYNAMIC_MESH_H__