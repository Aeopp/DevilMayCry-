#ifndef __STATIC_MESH_H__
#define __STATIC_MESH_H__
#include "Mesh.h"
BEGIN(ENGINE)
class ENGINE_DLL StaticMesh final : public Mesh
{
	//Subset은 원본과 복사본 모두 공유
	//Material과 Material에 속한 Texture는 복사본 개별 적용.
private:
private:
	explicit StaticMesh(LPDIRECT3DDEVICE9 const _pDevice);
	explicit StaticMesh(const StaticMesh& _rOther);
	virtual ~StaticMesh() = default;
	// Mesh을(를) 통해 상속됨
	virtual void Free() override;
public:
	static StaticMesh* Create(LPDIRECT3DDEVICE9 const _pDevice, const TSTRING& _sFilePath);
	// Mesh을(를) 통해 상속됨
	virtual Resource* Clone() override;

private:
	HRESULT LoadMeshFromFile(const std::filesystem::path _sPath);
};
END
#endif // !__STATIC_MESH_H__