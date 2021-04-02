#include "AssimpHelper.hpp"
#include "SkeletonMesh.h"
#include "Subset.h"

USING(ENGINE)

SkeletonMesh::SkeletonMesh(LPDIRECT3DDEVICE9 const _pDevice)
	: StaticMesh(_pDevice)
{
}

SkeletonMesh::SkeletonMesh(const StaticMesh& _rOther)
	: StaticMesh(_rOther)
{
}

void SkeletonMesh::Free()
{
	StaticMesh::Free();
};

SkeletonMesh* SkeletonMesh::Create(LPDIRECT3DDEVICE9 const _pDevice, 
								   const std::filesystem::path _Path)
{
	SkeletonMesh* pInstance = new SkeletonMesh(_pDevice);
	if (FAILED(pInstance->LoadMeshFromFile(_Path)))
	{
		pInstance->Free();
		delete pInstance;
		return nullptr;
	}
	return pInstance;
}

Resource* SkeletonMesh::Clone()
{
	SkeletonMesh* pClone = new SkeletonMesh(*this);
	return pClone;
}

HRESULT SkeletonMesh::LoadMeshFromFile(const std::filesystem::path _Path)&
{
	//Assimp Importer ����.
	auto AiImporter = Assimp::Importer{};
	//FBX������ �о Scene ����.
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

	return LoadSkeletonMeshImplementation(AiScene, _Path);


}

HRESULT SkeletonMesh::LoadSkeletonMeshImplementation(const aiScene* AiScene, const std::filesystem::path _Path)
{
	//Subset�� �����ϴ� vector �޸� ���� Ȯ��.
	m_vecSubset.resize(AiScene->mNumMeshes);

	//FBX�� Scene�� �����ϴ� Mesh(Subset)�� ���� ������ ����.
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
			&tVBDesc, &pVB, &pIB)))
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

Bone* SkeletonMesh::MakeHierarchy(Bone* BoneParent, const aiNode* const AiNode)
{
	auto TargetBone = std::make_shared<Bone>();
	BoneTable.push_back(TargetBone);
	const uint64 CurBoneIdx = BoneTable.size() - 1;
	const std::string InsertBoneName = AiNode->mName.C_Str();
	// �����ϸ� �̸� ���� ��� �̸� �����ؼ� �����ϴ� ���� �߰� .
	if (BoneTableIdxFromName.contains(InsertBoneName))
	{

	}
	BoneTableIdxFromName.insert({ AiNode->mName.C_Str()  ,CurBoneIdx });
	TargetBone->Name = AiNode->mName.C_Str();
	TargetBone->OriginTransform = TargetBone->Transform = AssimpHelper::ConvertMatrix(AiNode->mTransformation);
	TargetBone->Parent = BoneParent;
	TargetBone->ToRoot = TargetBone->OriginTransform * BoneParent->ToRoot;

	for (uint32 i = 0; i < AiNode->mNumChildren; ++i)
	{
		TargetBone->Childrens.push_back(MakeHierarchy(TargetBone.get(), AiNode->mChildren[i]));
	}

	return TargetBone.get();
}
