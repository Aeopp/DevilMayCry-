#ifndef __MESH_RENDERER_H__
#define __MESH_RENDERER_H__
#include "Component.h"
BEGIN(ENGINE)
class StaticMesh;
class ENGINE_DLL MeshRenderer final : public Component
{
private:
	std::shared_ptr<StaticMesh>	m_pStaticMesh;
private:
	explicit MeshRenderer(std::weak_ptr<GameObject> const _pGameObject);
	virtual ~MeshRenderer() = default;
	// Component을(를) 통해 상속됨
	virtual void Free() override;
public:
	static MeshRenderer* Create(std::weak_ptr<GameObject> const _pGameObject);
public:
	HRESULT Render();
public:
	HRESULT SetMesh(const std::filesystem::path _Path);

	const VERTEXBUFFERDESC& GetVertexBufferDesc(const UINT _nSubsetIdx);
	const MATERIAL&			GetMaterial(const UINT _nSubsetIdx);

};
END
#endif // !__MESH_RENDERER_H__