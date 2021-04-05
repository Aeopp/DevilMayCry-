#include "AssimpHelper.hpp"
#include "StaticMesh.h"
#include "Subset.h"

USING(ENGINE)

StaticMesh::StaticMesh(LPDIRECT3DDEVICE9 const _pDevice)
	: Mesh(_pDevice){}

StaticMesh::StaticMesh(const StaticMesh& _rOther)
	: Mesh(_rOther){}

void StaticMesh::Free()
{
	Mesh::Free();
}

StaticMesh* StaticMesh::Create(LPDIRECT3DDEVICE9 const _pDevice, const std::filesystem::path _Path)
{
	StaticMesh* pInstance = new StaticMesh(_pDevice);
	if (FAILED(pInstance->LoadMeshFromFile(_Path)))
	{
		pInstance->Free();
		delete pInstance;
		return nullptr;
	}
	return pInstance;
}

Resource* StaticMesh::Clone()
{
	StaticMesh* pClone = new StaticMesh(*this);
	return pClone;
}
void StaticMesh::Editor()
{
	Mesh::Editor();
	if (bEdit)
	{

	}
}
std::string StaticMesh::GetName()
{
	return "StaticMesh";
};

HRESULT StaticMesh::LoadMeshFromFile(const std::filesystem::path _Path)
{
	//Assimp Importer 생성.
	auto AiImporter = Assimp::Importer{};
	ResourcePath = _Path;
	const aiScene* const AiScene = AiImporter.ReadFile(	
		_Path.string(),
		aiProcess_MakeLeftHanded |
		aiProcess_FlipUVs |
		aiProcess_FlipWindingOrder |
		aiProcess_Triangulate |
		aiProcess_CalcTangentSpace |
		aiProcess_ValidateDataStructure |
		aiProcess_ImproveCacheLocality |
		aiProcess_RemoveRedundantMaterials |
		aiProcess_GenUVCoords |
		aiProcess_TransformUVCoords |
		aiProcess_FindInstances |
		aiProcess_GenSmoothNormals |
		aiProcess_SortByPType |
		aiProcess_OptimizeMeshes |
		aiProcess_SplitLargeMeshes
	);
	return LoadStaticMeshImplementation(AiScene, ResourcePath);
}
HRESULT StaticMesh::LoadStaticMeshImplementation(const aiScene* AiScene ,
										const std::filesystem::path _Path)
{
	//Subset을 보관하는 vector 메모리 공간 확보.
	m_vecSubset.resize(AiScene->mNumMeshes);

	//FBX의 Scene을 구성하는 Mesh(Subset)로 부터 데이터 구성.
	for (uint32 MeshIdx = 0u; MeshIdx < AiScene->mNumMeshes; ++MeshIdx)
	{
		//
		const auto* const AiMesh = AiScene->mMeshes[MeshIdx];
		//
		std::shared_ptr<Subset> _CurrentSubset;
		_CurrentSubset.reset(Subset::Create(m_pDevice), Deleter<Object>());

		VERTEXBUFFERDESC tVBDesc;
		LPDIRECT3DVERTEXBUFFER9 pVB = nullptr;
		LPDIRECT3DINDEXBUFFER9	pIB = nullptr;

		if (FAILED(AssimpHelper::LoadMesh(AiMesh, m_pDevice,
			&tVBDesc, &pVB, &pIB,nullptr)))
			return E_FAIL;

		MATERIAL tMaterial;

		if (FAILED(AssimpHelper::LoadMaterial(
			AiScene,
			AiMesh,
			{
				aiTextureType::aiTextureType_AMBIENT,
				aiTextureType::aiTextureType_AMBIENT_OCCLUSION,
				aiTextureType::aiTextureType_BASE_COLOR,
				aiTextureType::aiTextureType_DIFFUSE,
				aiTextureType::aiTextureType_DIFFUSE_ROUGHNESS,
				aiTextureType::aiTextureType_DISPLACEMENT,
				aiTextureType::aiTextureType_EMISSION_COLOR,
				aiTextureType::aiTextureType_EMISSIVE,
				aiTextureType::aiTextureType_HEIGHT,
				aiTextureType::aiTextureType_LIGHTMAP,
				aiTextureType::aiTextureType_METALNESS,
				aiTextureType::aiTextureType_NORMALS,
				aiTextureType::aiTextureType_NORMAL_CAMERA,
				aiTextureType::aiTextureType_OPACITY,
				aiTextureType::aiTextureType_REFLECTION,
				aiTextureType::aiTextureType_SHININESS,
				aiTextureType::aiTextureType_SPECULAR,
				aiTextureType::aiTextureType_UNKNOWN
			},
			_Path.parent_path(),
			m_pDevice,
			&tMaterial
		)))
		{
			SafeRelease(pVB);
			SafeRelease(pIB);
			return E_FAIL;
		}

		SafeAddRef(tVBDesc.pVertexDecl);

		_CurrentSubset->Initialize(pVB, pIB, tVBDesc, tMaterial);
		m_vecSubset[MeshIdx] = _CurrentSubset;
	};

	MakeVertexLcationsFromSubset();



	return S_OK;
}



