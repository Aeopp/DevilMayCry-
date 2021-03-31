#ifndef __STATIC_MESH_H__
#define __STATIC_MESH_H__
#include "AssimpHelper.hpp"
#include "StringHelper.hpp"
#include "Subset.h"
#include "Mesh.h"
#include <memory>
#include <vector>
#include "EngineDefine.h"
#include <array>
#include "TextureInformation.hpp"

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
	static StaticMesh* Create(LPDIRECT3DDEVICE9 const _pDevice, const std::filesystem::path _Path);
	// Mesh을(를) 통해 상속됨
	virtual Resource* Clone() override;
	HRESULT LoadMeshFromFile(const std::filesystem::path _Path)&;
};

END
#endif // !__STATIC_MESH_H__