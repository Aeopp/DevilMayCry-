#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "StaticMesh.h"

USING(ENGINE)

StaticMesh::StaticMesh(LPDIRECT3DDEVICE9 const _pDevice)
	: Mesh(_pDevice)
{
}

StaticMesh::StaticMesh(const StaticMesh& _rOther)
	: Mesh(_rOther.m_pDevice)
{
}

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


HRESULT StaticMesh::LoadMeshFromFile(const std::filesystem::path _Path)&
{
	// 컴파일 에러  .

	Assimp::Importer AiImporter;
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

	LocalVertexLocations = std::make_shared<std::vector<Vector3>>();
	//SubSets = std::make_shared<std::vector<Subset>>();
	//SubSets->resize(AiScene->mNumMeshes);
	SubSets.resize(AiScene->mNumMeshes);

	for (uint32 MeshIdx = 0u; MeshIdx < AiScene->mNumMeshes; ++MeshIdx)
	{
		const auto *const AiMesh = AiScene->mMeshes[MeshIdx];

		//???
		std::shared_ptr<Subset> _CurrentSubset /*=
			std::make_shared<Subset>(Subset::Create(m_pDevice), Deleter<Object>())*/;
		_CurrentSubset.reset(Subset::Create(m_pDevice), Deleter<Object>());

		const VertexInformation _VertexInformation = AssimpHelper::LoadVertexInformation(AiMesh, m_pDevice);

		const Material CurLoadMaterial =
			AssimpHelper::LoadMaterialFromMesh(
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

				_Path.parent_path(), m_pDevice);

		_CurrentSubset->Initialize(
			_VertexInformation ,
			CurLoadMaterial);
	};

	return S_OK;
}



