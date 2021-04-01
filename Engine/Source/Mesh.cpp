#include "Mesh.h"
#include "Subset.h"

USING(ENGINE)

Mesh::Mesh(LPDIRECT3DDEVICE9 const _pDevice)
	: Resource(_pDevice)
{
}

Mesh::Mesh(const Mesh& _rOther)
	: Resource(_rOther.m_pDevice)
	, m_vecSubset(_rOther.m_vecSubset)
{
}
void Mesh::Free()
{
	m_vecSubset.clear();
	m_vecSubset.shrink_to_fit();
	Resource::Free();
}

HRESULT Mesh::Render()
{
	for (auto& pSubset : m_vecSubset)
	{
		if (nullptr == pSubset)
			continue;
		pSubset->Render();
	}
}

std::weak_ptr<Subset> Mesh::GetSubset(const UINT _nIndex)
{
	if (_nIndex >= m_vecSubset.size())
		return std::weak_ptr<Subset>();

	return m_vecSubset[_nIndex];
}

const VERTEXBUFFERDESC& Mesh::GetVertexBufferDesc(const UINT _nSubsetIdx)
{
	// TODO: 여기에 return 문을 삽입합니다.
	if (_nSubsetIdx >= m_vecSubset.size())
		return VERTEXBUFFERDESC();

	return m_vecSubset[_nSubsetIdx]->GetVertexBufferDesc();
}

const MATERIAL& Mesh::GetMaterial(const UINT _nSubsetIdx)
{
	// TODO: 여기에 return 문을 삽입합니다.
	if (_nSubsetIdx >= m_vecSubset.size())
		return MATERIAL();

	return m_vecSubset[_nSubsetIdx]->GetMaterial();
}
