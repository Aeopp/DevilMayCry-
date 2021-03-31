#ifndef __ASSIMP_HELPER_H__
#define __ASSIMP_HELPER_H__
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "EngineStdafx.h"

#include <iostream>

#include "Resources.h"
#include "Texture.h"

BEGIN(ENGINE)
BEGIN(AssimpHelper)
static inline void DebugPrint(const aiMatrix4x4& AiMatrix)
{
	for (uint32 Row = 0u; Row < 4u; ++Row)
	{
		for (uint32 Col = 0u; Col < 4u; ++Col)
		{
			std::cout << AiMatrix[Row][Col] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

// ������ ����� ��ġ�� DX�� ����� ��ķ� ��ȯ�Ѵ�.
static inline Matrix ConvertMatrix(const aiMatrix4x4& AiMatrix)
{
	Matrix _Matrix;
	std::memcpy(&_Matrix, &AiMatrix, sizeof(aiMatrix4x4));
	D3DXMatrixTranspose(&_Matrix, &_Matrix);
	return _Matrix;
};

static inline Vector3 ConvertVec3(const aiVector3D& AiVector3D)
{
	return Vector3
	{
		AiVector3D.x ,
		AiVector3D.y ,
		AiVector3D.z
	};
};

static inline Quaternion ConvertQuat(const aiQuaternion& AiQuaternion)
{
	Quaternion _Quat;
	_Quat.x = AiQuaternion.x;
	_Quat.y = AiQuaternion.y;
	_Quat.z = AiQuaternion.z;
	_Quat.w = AiQuaternion.w;
	return _Quat;
};


static inline HRESULT LoadMesh(
	IN	const aiMesh* const				_pAiMesh,
	IN	LPDIRECT3DDEVICE9 const			_pDevice,
	OUT LPVERTEXBUFFERDESC const		_pVBDesc,
	OUT	LPDIRECT3DVERTEXBUFFER9* const	_ppVB,
	OUT LPDIRECT3DINDEXBUFFER9*	const	_ppIB)
{
#pragma region SET VERTEX BUFFER DESCRIPTION
	_pVBDesc->nNumFaces				= _pAiMesh->mNumFaces;
	_pVBDesc->nNumVertices			= _pAiMesh->mNumVertices;
	_pVBDesc->nNumUVChannel			= _pAiMesh->GetNumUVChannels();
	_pVBDesc->bHasPosition			= _pAiMesh->HasPositions();
	_pVBDesc->bHasNormal			= _pAiMesh->HasNormals();
	_pVBDesc->bHasTangentBiNormal	= _pAiMesh->HasTangentsAndBitangents();

	_pVBDesc->vecNumUVComponents.reserve(_pVBDesc->nNumUVChannel);
#pragma endregion

#pragma region SET VERTEXELEMENT & CREATE VERTEXDECLARATION
	std::vector<D3DVERTEXELEMENT9> vecElements;

	if (_pVBDesc->bHasPosition)
	{
		vecElements.push_back
		(
			D3DVERTEXELEMENT9
			{
				0,
				(WORD)_pVBDesc->nStride,
				D3DDECLTYPE_FLOAT3,
				D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_POSITION,
				0
			}
		);
		_pVBDesc->nStride += (sizeof(float) * 3u);
	}

	if (_pVBDesc->bHasNormal)
	{
		vecElements.push_back
		(
			D3DVERTEXELEMENT9
			{
				0,
				(WORD)_pVBDesc->nStride,
				D3DDECLTYPE_FLOAT3,
				D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_NORMAL,
				0
			}
		);
		_pVBDesc->nStride += (sizeof(float) * 3u);
	}

	if (_pVBDesc->bHasTangentBiNormal)
	{
		vecElements.push_back
		(
			D3DVERTEXELEMENT9
			{
				0,
				(WORD)_pVBDesc->nStride,
				D3DDECLTYPE_FLOAT3,
				D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_TANGENT,
				0
			}
		);
		_pVBDesc->nStride += (sizeof(float) * 3u);

		vecElements.push_back
		(
			D3DVERTEXELEMENT9
			{
				0,
				(WORD)_pVBDesc->nStride,
				D3DDECLTYPE_FLOAT3,
				D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_BINORMAL,
				0
			}
		);
		_pVBDesc->nStride += (sizeof(float) * 3u);
	}

	for (BYTE i = 0; i < _pVBDesc->nNumUVChannel; ++i)
	{
		_pVBDesc->vecNumUVComponents.push_back(_pAiMesh->mNumUVComponents[i]);

		D3DDECLTYPE eDeclType = D3DDECLTYPE_FLOAT2;
		//eDeclType = (D3DDECLTYPE)(_pVBDesc->vecNumUVComponents[i] - 1);
		switch (_pVBDesc->vecNumUVComponents[i])
		{
		case 1:
			eDeclType = D3DDECLTYPE_FLOAT1;
			break;
		case 2:
			eDeclType = D3DDECLTYPE_FLOAT2;
			break;
		case 3:
			eDeclType = D3DDECLTYPE_FLOAT3;
			break;
		case 4:
			eDeclType = D3DDECLTYPE_FLOAT4;
			break;
		default:
			break;
		}

		vecElements.push_back
		(
			D3DVERTEXELEMENT9
			{
				0,
				(WORD)_pVBDesc->nStride,
				(BYTE)eDeclType,
				D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_TEXCOORD,
				i
			}
		);
		_pVBDesc->nStride += (sizeof(float) * _pVBDesc->vecNumUVComponents[i]);
	}

	vecElements.push_back(D3DDECL_END());

	if (FAILED(_pDevice->CreateVertexDeclaration(vecElements.data(), &(_pVBDesc->pVertexDecl))))
	{
		PRINT_LOG(TEXT("Failed to load mesh from AiMesh."), TEXT("Failed to CreateVertexDeclaration"));
		return E_FAIL;
	}
#pragma endregion

#pragma region CREATE VERTEX BUFFER

	//_pAiMesh�� ���� �޽��� �̷�� ���� ���� �ε�
	std::vector<float>	vecVertices;

	for (UINT nVertexIdx = 0; nVertexIdx < _pVBDesc->nNumVertices; ++nVertexIdx)
	{
		if (_pVBDesc->bHasPosition)
		{
			vecVertices.push_back(_pAiMesh->mVertices[nVertexIdx].x);
			vecVertices.push_back(_pAiMesh->mVertices[nVertexIdx].y);
			vecVertices.push_back(_pAiMesh->mVertices[nVertexIdx].z);
			_pVBDesc->LocalVertexLocation->push_back(AssimpHelper::ConvertVec3(_pAiMesh->mVertices[nVertexIdx]));
		}

		if (_pVBDesc->bHasNormal)
		{
			vecVertices.push_back(_pAiMesh->mNormals[nVertexIdx].x);
			vecVertices.push_back(_pAiMesh->mNormals[nVertexIdx].y);
			vecVertices.push_back(_pAiMesh->mNormals[nVertexIdx].z);
		}

		if (_pVBDesc->bHasTangentBiNormal)
		{
			vecVertices.push_back(_pAiMesh->mTangents[nVertexIdx].x);
			vecVertices.push_back(_pAiMesh->mTangents[nVertexIdx].y);
			vecVertices.push_back(_pAiMesh->mTangents[nVertexIdx].z);

			vecVertices.push_back(_pAiMesh->mBitangents[nVertexIdx].x);
			vecVertices.push_back(_pAiMesh->mBitangents[nVertexIdx].y);
			vecVertices.push_back(_pAiMesh->mBitangents[nVertexIdx].z);
		}

		for (uint32 nChannelIdx = 0; nChannelIdx < _pVBDesc->nNumUVChannel; ++nChannelIdx)
		{
			const uint32 nCurNumUVComponents = _pVBDesc->vecNumUVComponents[nChannelIdx];
			const float* TexCoordPtr = reinterpret_cast<const float*>(&(_pAiMesh->mTextureCoords[nChannelIdx][nVertexIdx]));
			
			for (uint32 i = 0; i < nCurNumUVComponents; ++i)
				vecVertices.push_back(TexCoordPtr[i]);
		}
	}

	//���� ���� ����
	_pVBDesc->nBufferSize = _pVBDesc->nNumVertices * _pVBDesc->nStride;
		
	if (FAILED(_pDevice->CreateVertexBuffer(_pVBDesc->nBufferSize, D3DUSAGE_WRITEONLY, NULL, D3DPOOL_MANAGED, _ppVB, nullptr)))
	{
		PRINT_LOG(TEXT("Failed to load mesh from AiMesh."), TEXT("Failed to CreateVertexBuffer"));
		return E_FAIL;
	}
	//���� ���� ������ ����.
	void* pVertices = nullptr;
	(*_ppVB)->Lock(0, 0, (void**)pVertices, 0);
	memcpy(pVertices, vecVertices.data(), (size_t)_pVBDesc->nBufferSize);
	(*_ppVB)->Unlock();


#pragma endregion

#pragma region CREATE INDEX BUFFER
	//_pAiMesh�� ���� �޽��� �̷�� ���� ���� �ε��� ���� �ε�.
	std::vector<uint32> vecIndices;

	for (uint32 nFaceIdx = 0; nFaceIdx < _pVBDesc->nNumFaces; ++nFaceIdx)
	{
		const aiFace CurrFace = _pAiMesh->mFaces[nFaceIdx];
		for (uint32 nIdx = 0; nIdx < CurrFace.mNumIndices; ++nIdx)
			vecIndices.push_back(CurrFace.mIndices[nIdx]);
	}
	//�ε��� ���� ����
	const uint32 IdxBufSize = sizeof(uint32) * vecIndices.size();
	
	if (FAILED(_pDevice->CreateIndexBuffer(IdxBufSize, D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_MANAGED, _ppIB, nullptr)))
	{
		SafeRelease(*_ppVB);
		PRINT_LOG(TEXT("Failed to load mesh from AiMesh."), TEXT("Failed to CreateIndexBuffer"));
		return E_FAIL;
	}
	//�ε��� ���� ������ ����
	uint32* pIndices = nullptr;
	(*_ppIB)->Lock(0, 0, (void**)pIndices, 0);
	memcpy(pIndices, vecIndices.data(), IdxBufSize);
	(*_ppIB)->Unlock();
#pragma endregion
	return S_OK;
}

static inline HRESULT LoadMaterial(
	IN const aiScene* const				_pAiScene,
	IN const aiMesh* const				_pAiMesh,
	IN const std::set<aiTextureType>&	_AiTexTypes,
	IN const std::filesystem::path&		_Path,
	IN const LPDIRECT3DDEVICE9			_pDevice,
	OUT LPMATERIAL						_pMaterial)
{
	if (false == _pAiScene->HasMaterials())
		return S_OK;

	//
	aiMaterial* pAiMaterial = _pAiScene->mMaterials[_pAiMesh->mMaterialIndex];
	//���� �̸� ����.
	std::filesystem::path sBuf = pAiMaterial->GetName().C_Str();
#ifdef UNICODE
	std::wstring sName = sBuf.wstring();
#else
	std::string sName = sBuf.string();
#endif
	_pMaterial->sName = sName;

	//���� ���� ����
	aiColor4D color;
	pAiMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color);
	_pMaterial->tMaterial.Diffuse = D3DCOLORVALUE(color.r, color.g, color.b, color.a);
	pAiMaterial->Get(AI_MATKEY_COLOR_SPECULAR, color);
	_pMaterial->tMaterial.Specular = D3DCOLORVALUE(color.r, color.g, color.b, color.a);
	pAiMaterial->Get(AI_MATKEY_COLOR_AMBIENT, color);
	_pMaterial->tMaterial.Ambient = D3DCOLORVALUE(color.r, color.g, color.b, color.a);
	pAiMaterial->Get(AI_MATKEY_COLOR_EMISSIVE, color);
	_pMaterial->tMaterial.Emissive = D3DCOLORVALUE(color.r, color.g, color.b, color.a);
	_pMaterial->tMaterial.Power = 1.f;

	//���� �ؽ�ó �ε�.
	for (const auto& aiTexType : _AiTexTypes)
	{
		const uint32 nNumTypeTex = pAiMaterial->GetTextureCount(aiTexType);

		if (0 == nNumTypeTex)
			continue;

		//���� �ؽ�ó�� ������ �����̳��� �޸� Ȯ��.
		_pMaterial->Textures[(UINT)aiTexType].resize(nNumTypeTex);

		for (uint32 nTexIdx = 0; nTexIdx < nNumTypeTex; ++nTexIdx)
		{
			aiString sAiPath;

			TEXTUREDESC			tTextureDesc;

			aiTextureMapping	eAiTextureMapping;
			unsigned int		nUVIdx;
			ai_real				fBlendFactor;
			aiTextureOp			eAiTextureOp;
			aiTextureMapMode	eAiTextureMapMode;

			if (AI_SUCCESS == pAiMaterial->GetTexture(
				aiTexType,
				nTexIdx,
				&sAiPath,
				&eAiTextureMapping,
				&nUVIdx,
				&fBlendFactor,
				&eAiTextureOp,
				&eAiTextureMapMode))
			{
				tTextureDesc.eMappingMode		= eAiTextureMapMode;
				tTextureDesc.eMappingType		= eAiTextureMapping;
				tTextureDesc.eTextureOperator	= eAiTextureOp;
				tTextureDesc.fBlendFactor		= fBlendFactor;
				tTextureDesc.nUVIdx				= nUVIdx;

				const std::filesystem::path FileNamePath = sAiPath.C_Str();
				const std::filesystem::path	CurTexFilePath = _Path / FileNamePath.filename();

				std::shared_ptr<Texture> pTexture = Resources::Load<Texture>(CurTexFilePath.wstring());

				pTexture->SetDesc(tTextureDesc);

				if (nullptr != pTexture)
					_pMaterial->Textures[nTexIdx].push_back(pTexture);
			}
		}
	}
	return S_OK;
}

END
END


#endif // !__ASSIMP_HELPER_H__