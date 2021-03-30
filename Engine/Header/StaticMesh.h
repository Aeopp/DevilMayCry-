#ifndef __STATIC_MESH_H__
#define __STATIC_MESH_H__
#include "Mesh.h"
BEGIN(ENGINE)
class ENGINE_DLL StaticMesh final : public Mesh
{
	//Subset�� ������ ���纻 ��� ����
	//Material�� Material�� ���� Texture�� ���纻 ���� ����.
private:
private:
	explicit StaticMesh(LPDIRECT3DDEVICE9 const _pDevice);
	explicit StaticMesh(const StaticMesh& _rOther);
	virtual ~StaticMesh() = default;
	// Mesh��(��) ���� ��ӵ�
	virtual void Free() override;
public:
	static StaticMesh* Create(LPDIRECT3DDEVICE9 const _pDevice, const TSTRING& _sFilePath);
	// Mesh��(��) ���� ��ӵ�
	virtual Resource* Clone() override;

private:
	HRESULT LoadMeshFromFile(const std::filesystem::path _sPath);
};
END
#endif // !__STATIC_MESH_H__