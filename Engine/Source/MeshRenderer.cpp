#include "MeshRenderer.h"
#include "StaticMesh.h"
#include "Resources.h"

USING(ENGINE)

MeshRenderer::MeshRenderer(std::weak_ptr<GameObject> const _pGameObject)
	: Component(_pGameObject)
{
}

void MeshRenderer::Free()
{
	m_pStaticMesh.reset();
	Component::Free();
}

MeshRenderer* MeshRenderer::Create(std::weak_ptr<GameObject> const _pGameObject)
{
	MeshRenderer* pInstance = new MeshRenderer(_pGameObject);
	return pInstance;
}

HRESULT MeshRenderer::Render()
{
	if (nullptr == m_pStaticMesh)
		return S_OK;

	return m_pStaticMesh->Render();
}

HRESULT MeshRenderer::SetMesh(const std::filesystem::path _Path)
{
	m_pStaticMesh = Resources::Load<StaticMesh>(_Path);

	if (nullptr == m_pStaticMesh)
		return E_FAIL;

	return S_OK;
}

const VERTEXBUFFERDESC& MeshRenderer::GetVertexBufferDesc(const UINT _nSubsetIdx)
{
	// TODO: 여기에 return 문을 삽입합니다.
	if (nullptr == m_pStaticMesh)
		return VERTEXBUFFERDESC();

	return m_pStaticMesh->GetVertexBufferDesc(_nSubsetIdx);
}

const MATERIAL& MeshRenderer::GetMaterial(const UINT _nSubsetIdx)
{
	// TODO: 여기에 return 문을 삽입합니다.
	if (nullptr == m_pStaticMesh)
		return MATERIAL();

	return m_pStaticMesh->GetMaterial(_nSubsetIdx);
}
