#include "Mesh.h"
#include "Subset.h"
#include "FMath.hpp"

USING(ENGINE)

Mesh::Mesh(LPDIRECT3DDEVICE9 const _pDevice)
	: Resource(_pDevice)
{

}

Mesh::Mesh(const Mesh& _rOther)
	: Resource(_rOther)
	, m_vecSubset(_rOther.m_vecSubset)
	, m_spVertexLocations(_rOther.m_spVertexLocations)
{
}

void Mesh::MakeVertexLcationsFromSubset()&
{
	m_spVertexLocations = std::make_shared<std::vector<Vector3>>();

	for (const auto& CurSubset : m_vecSubset)
	{
		if (CurSubset)
		{
			const auto& CurLocalVertexLocations = CurSubset->GetVertexBufferDesc().LocalVertexLocation;
			if (CurLocalVertexLocations)
			{
				for (const Vector3 LocalVertexLocation : *CurLocalVertexLocations)
				{
					m_spVertexLocations->push_back(LocalVertexLocation);
				}
			}
		}
	}
};

void Mesh::Free()
{
	m_vecSubset.clear();
	m_vecSubset.shrink_to_fit();
	Resource::Free();
}

HRESULT Mesh::Render(ID3DXEffect* const Fx)
{
	for (auto& pSubset : m_vecSubset)
	{
		if (nullptr == pSubset)
			continue;
		pSubset->Render(Fx);
	}

	return S_OK;
}

const uint64 Mesh::GetNumSubset() const
{
	return m_vecSubset.size();
}

std::weak_ptr<Subset> Mesh::GetSubset(const UINT _nIndex)
{
	if (_nIndex >= m_vecSubset.size())
		return std::weak_ptr<Subset>();

	return m_vecSubset[_nIndex];
}

void Mesh::Editor()
{
	Resource::Editor();
	if (bEdit)
	{
		for (auto& _Subset:m_vecSubset)
		{
			_Subset->Editor();
		}
	}
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
