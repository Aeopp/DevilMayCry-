#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "StaticMesh.h"

#include "Subset.h"

#include "Resources.h"
#include "Texture.h"

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

StaticMesh* StaticMesh::Create(LPDIRECT3DDEVICE9 const _pDevice, const TSTRING& _sFilePath)
{
	StaticMesh* pInstance = new StaticMesh(_pDevice);
	if (FAILED(pInstance->LoadMeshFromFile(_sFilePath)))
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

HRESULT StaticMesh::LoadMeshFromFile(const std::filesystem::path _sPath)
{
	Assimp::Importer importer;
	
	const aiScene* pAiScene = importer.ReadFile(_sPath.string(),
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
		aiProcess_SplitLargeMeshes);

	for (UINT nMeshIdx = 0; nMeshIdx < pAiScene->mNumMeshes; ++nMeshIdx)
	{
		aiMesh* pAiMesh = pAiScene->mMeshes[nMeshIdx];
		//정점 정보 로드
		std::vector<STATICVERTEX> vecVertices;
		for (UINT nVertexIdx = 0; nVertexIdx < pAiMesh->mNumVertices; ++nVertexIdx)
		{
			STATICVERTEX tVertex;
			ZeroMemory(&tVertex, sizeof(STATICVERTEX));
			if (true == pAiMesh->HasPositions())
				memcpy_s(&tVertex.vPosition, sizeof(D3DXVECTOR3), &pAiMesh->mVertices[nVertexIdx], sizeof(D3DXVECTOR3));
			if (true == pAiMesh->HasNormals())
				memcpy_s(&tVertex.vNormal, sizeof(D3DXVECTOR3), &pAiMesh->mNormals[nVertexIdx], sizeof(D3DXVECTOR3));
			if (true == pAiMesh->HasTangentsAndBitangents())
			{
				memcpy_s(&tVertex.vBiNormal, sizeof(D3DXVECTOR3), &pAiMesh->mBitangents[nVertexIdx], sizeof(D3DXVECTOR3));
				memcpy_s(&tVertex.vTangent, sizeof(D3DXVECTOR3), &pAiMesh->mTangents[nVertexIdx], sizeof(D3DXVECTOR3));
			}
			if(true == pAiMesh->HasTextureCoords(0))
				memcpy_s(&tVertex.vUV, sizeof(D3DXVECTOR2), &pAiMesh->mTextureCoords[0][nVertexIdx], sizeof(D3DXVECTOR2));
			
			vecVertices.push_back(tVertex);
		}
		
		//정점 버퍼 생성
		LPDIRECT3DVERTEXBUFFER9 pVB = nullptr;		
		m_pDevice->CreateVertexBuffer(sizeof(STATICVERTEX) * vecVertices.size(), D3DUSAGE_WRITEONLY, NULL, D3DPOOL_MANAGED, &pVB, nullptr);
		//정점 버퍼 데이터 설정.
		LPSTATICVERTEX pVertices = nullptr;
		pVB->Lock(0, 0, (void**)pVertices, NULL);
		memcpy(pVertices, vecVertices.data(), sizeof(STATICVERTEX) * vecVertices.size());
		pVB->Unlock();

		//인덱스 정보 로드
		std::vector<UINT> vecIndex;
		for (UINT nFaceIdx = 0; nFaceIdx < pAiMesh->mNumFaces; ++nFaceIdx)
		{
			const aiFace CurrentFace = pAiMesh->mFaces[nFaceIdx];
			for (UINT nIndexIdx = 0; nIndexIdx < CurrentFace.mNumIndices; ++nIndexIdx)
				vecIndex.push_back(CurrentFace.mIndices[nIndexIdx]);
		}
		//인덱스 버퍼 생성
		LPDIRECT3DINDEXBUFFER9 pIB = nullptr;
		m_pDevice->CreateIndexBuffer(sizeof(UINT) * vecIndex.size(), D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_MANAGED, &pIB, nullptr);
		//인덱스 버퍼 데이터 설정
		UINT* pIndices = nullptr;
		pIB->Lock(0, 0, (void**)&pIndices, NULL);
		memcpy(pIndices, vecIndex.data(), sizeof(UINT) * vecIndex.size());
		pIB->Unlock();
		
		//재질 정보 로드
		MATERIAL tMaterial;
		if (true == pAiScene->HasMaterials())
		{
			aiMaterial* pAiMaterial = pAiScene->mMaterials[pAiMesh->mMaterialIndex];
			aiColor4D color;

			pAiMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color);
			tMaterial.tMaterial.Diffuse = D3DCOLORVALUE(color.r, color.g, color.b, color.a);
			pAiMaterial->Get(AI_MATKEY_COLOR_SPECULAR, color);
			tMaterial.tMaterial.Specular = D3DCOLORVALUE(color.r, color.g, color.b, color.a);
			pAiMaterial->Get(AI_MATKEY_COLOR_AMBIENT, color);
			tMaterial.tMaterial.Ambient = D3DCOLORVALUE(color.r, color.g, color.b, color.a);
			pAiMaterial->Get(AI_MATKEY_COLOR_EMISSIVE, color);
			tMaterial.tMaterial.Emissive = D3DCOLORVALUE(color.r, color.g, color.b, color.a);

			aiString sAiPath;
			std::filesystem::path sPath;
			if (0 != pAiMaterial->GetTextureCount(aiTextureType::aiTextureType_DIFFUSE))
			{
				pAiMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &sAiPath);
				sPath = _sPath;			//파일명 제거한 경로만
				sPath += sAiPath.C_Str();//경로에 텍스처 파일명 더해서 최종경로 완성

				tMaterial.pDiffuse = Resources::Load<Texture>(sPath);
			}

			if (0 != pAiMaterial->GetTextureCount(aiTextureType::aiTextureType_SPECULAR))
			{
				pAiMaterial->GetTexture(aiTextureType_SPECULAR, 0, &sAiPath);
				sPath = _sPath;			//파일명 제거한 경로만
				sPath += sAiPath.C_Str();//경로에 텍스처 파일명 더해서 최종경로 완성

				tMaterial.pSpecular = Resources::Load<Texture>(sPath);
			}

			if (0 != pAiMaterial->GetTextureCount(aiTextureType::aiTextureType_NORMALS))
			{
				pAiMaterial->GetTexture(aiTextureType_NORMALS, 0, &sAiPath);
				sPath = _sPath;			//파일명 제거한 경로만
				sPath += sAiPath.C_Str();//경로에 텍스처 파일명 더해서 최종경로 완성

				tMaterial.pNormal = Resources::Load<Texture>(sPath);
			}

			m_vecMaterial.push_back(tMaterial);
		}
		
		Subset* pSubset = nullptr;

		pSubset = Subset::Create(m_pDevice);

		pSubset->Initialize(pVB, pIB, sizeof(STATICVERTEX), pAiMesh->mNumFaces, pAiMesh->mNumVertices, pAiMesh->mNumFaces);

		m_vecSubset.push_back(std::shared_ptr<Subset>(pSubset, Deleter<Object>()));
	}
	return S_OK;
}
