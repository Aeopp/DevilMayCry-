#ifndef __STATIC_MESH_H__
#define __STATIC_MESH_H__
#include "Mesh.h"
#include "StringHelper.hpp"
#include "AssimpHelper.hpp"
#include "Subset.h"
#include <memory>
#include <vector>
#include "EngineDefine.h"
#include <array>


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

HRESULT StaticMesh::LoadMeshFromFile(const std::filesystem::path _Path)&
{
	Assimp::Importer AiImporter;
	const aiScene* const AiScene =  AiImporter.ReadFile(
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
	SubSets = std::make_shared<std::vector<Subset>>();
	SubSets->resize(AiScene->mNumMeshes);
	
	for (uint32 MeshIdx = 0u; MeshIdx < AiScene->mNumMeshes; ++MeshIdx)
	{
		std::shared_ptr<Subset> _CurrentSubset = std::make_shared<Subset>(Subset::Create(m_pDevice),Deleter<Object>());

		aiMesh* _AiMesh = AiScene->mMeshes[MeshIdx];
		
		const uint32 NumVerticies = _AiMesh->mNumVertices;

		const bool HasPosition = _AiMesh->HasPositions();
		const bool HasNormal = _AiMesh->HasNormals();
		const bool HasTangentBiNormal = _AiMesh->HasTangentsAndBitangents();
		// UV 채널이 몇개인가 
		const uint32 NumUVChannel =_AiMesh->GetNumUVChannels();
		// UV 채널 인덱스의 컴포넌트가 몇개인지 
		std::vector<uint32> NumUVComponents{};
		NumUVComponents.resize(NumUVChannel);

		uint32 VertexSize = 0u;
		std::vector<D3DVERTEXELEMENT9> DeclElements{};
		
		if (HasPosition)
		{
			DeclElements.push_back
			(
				D3DVERTEXELEMENT9{ 0, VertexSize,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 }
			);
			VertexSize += (sizeof(float) * 3u);
		}
		if (HasNormal)
		{
			DeclElements.push_back
			(
				D3DVERTEXELEMENT9{ 0, VertexSize, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 }
			);
			VertexSize += (sizeof(float) * 3u);
		}
		if (HasTangentBiNormal)
		{
			DeclElements.push_back
			(
				D3DVERTEXELEMENT9{ 0, VertexSize, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0 }
			);
			VertexSize += (sizeof(float) * 3u);

			DeclElements.push_back
			(
				D3DVERTEXELEMENT9{ 0, VertexSize, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL,  0 }
			);

			VertexSize += (sizeof(float) * 3u);
		}

		for (uint32 i = 0u; i < NumUVChannel; ++i)
		{
			const uint32 NumUVComponent = _AiMesh->mNumUVComponents[i];
			D3DDECLTYPE NumDecltypeFloat = D3DDECLTYPE_FLOAT2;
			switch (NumUVComponent)
			{ 
			case 1: 
				NumDecltypeFloat = D3DDECLTYPE_FLOAT1; 
				break;
			case 2 :
				NumDecltypeFloat = D3DDECLTYPE_FLOAT2; 
				break;
			case 3 : 
				NumDecltypeFloat = D3DDECLTYPE_FLOAT3;  
				break;
			case 4:
				NumDecltypeFloat = D3DDECLTYPE_FLOAT4;
				break;
			default:
				break;
			}

			DeclElements.push_back
			(
				D3DVERTEXELEMENT9{ 0, VertexSize, NumDecltypeFloat, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,  i }
			);
			VertexSize += (sizeof(float) * NumUVComponent);
			NumUVComponents[i] = NumUVComponent;
		}
		DeclElements.push_back(D3DDECL_END());
		IDirect3DVertexDeclaration9* VertexDeclaration{ nullptr };
		m_pDevice->CreateVertexDeclaration(DeclElements.data(), &VertexDeclaration);
		
		// 버텍스 버퍼.
		std::vector<float> Verticies{};

		for(uint32 VerticesIdx = 0u; VerticesIdx < _AiMesh->mNumVertices; ++VerticesIdx)
		{
			if (HasPosition)
			{
				Verticies.push_back(
					_AiMesh->mVertices[VerticesIdx].x
				);

				Verticies.push_back(
					_AiMesh->mVertices[VerticesIdx].y
				);

				Verticies.push_back(
					_AiMesh->mVertices[VerticesIdx].z
				);

				LocalVertexLocations->push_back(FromAssimp(_AiMesh->mVertices[VerticesIdx]));
			}
			if (HasNormal)
			{
				Verticies.push_back(
					_AiMesh->mNormals[VerticesIdx].x
				);

				Verticies.push_back(
					_AiMesh->mNormals[VerticesIdx].y
				);

				Verticies.push_back(
					_AiMesh->mNormals[VerticesIdx].z
				);
			}
			if (HasTangentBiNormal)
			{
				Verticies.push_back(
					_AiMesh->mTangents[VerticesIdx].x
				);

				Verticies.push_back(
					_AiMesh->mTangents[VerticesIdx].y
				);

				Verticies.push_back(
					_AiMesh->mTangents[VerticesIdx].z
				);


				Verticies.push_back(
					_AiMesh->mBitangents[VerticesIdx].x
				);

				Verticies.push_back(
					_AiMesh->mBitangents[VerticesIdx].y
				);

				Verticies.push_back(
					_AiMesh->mBitangents[VerticesIdx].z
				);
			}
			for (uint32 i = 0u; i < NumUVChannel; ++i)
			{
				const uint32 CurNumUVComponents = NumUVComponents[i];
				const float* TexCoordPtr = reinterpret_cast<const float*const>(&_AiMesh->mTextureCoords[i][VerticesIdx]);
				for (uint32 j = 0u; j < CurNumUVComponents; ++j)
				{
					Verticies.push_back(TexCoordPtr[j]);
				}
			}
		};

		IDirect3DVertexBuffer9* _VertexBuffer{ nullptr };
		const uint32 VtxBufSize = VertexSize * NumVerticies;
		m_pDevice->CreateVertexBuffer(VtxBufSize,D3DUSAGE_WRITEONLY,NULL,D3DPOOL_MANAGED,&_VertexBuffer,nullptr);
		const uint32 NumVerticies = _AiMesh->mNumFaces * 3u;
		void *VertexBufferPtr{ nullptr };
		_VertexBuffer->Lock(0, 0,(&VertexBufferPtr), NULL);
		std::memcpy(VertexBufferPtr, Verticies.data(), VtxBufSize);
		_VertexBuffer->Unlock();

		std::vector<uint32> Indicies{};

		IDirect3DIndexBuffer9* _IndexBuffer{ nullptr };
		const uint32 NumFaces = _AiMesh->mNumFaces; 
		for (uint32 FaceIdx = 0u; FaceIdx < NumFaces; ++FaceIdx)
		{
			const aiFace CurrentFace = _AiMesh->mFaces[FaceIdx];
			for (uint32 Idx = 0u; Idx < CurrentFace.mNumIndices; ++Idx)
			{
				Indicies.push_back(CurrentFace.mIndices[Idx]);
			}
			const uint32 IdxBufSize = sizeof(uint32) * Indicies.size();
			m_pDevice->CreateIndexBuffer(IdxBufSize, D3DUSAGE_WRITEONLY, D3DFMT_INDEX32,
				D3DPOOL_MANAGED, &_IndexBuffer, nullptr);
			uint32* IndexBufferPtr{ nullptr };
			_IndexBuffer->Lock(0, 0, reinterpret_cast<void**>(&IndexBufferPtr), NULL);
			std::memcpy(IndexBufferPtr, Indicies.data(), IdxBufSize);
			_IndexBuffer->Unlock();

			aiMaterial* AiMaterial = AiScene->mMaterials[_AiMesh->mMaterialIndex];
		};

		if (AiScene->HasMaterials())
		{
			const std::string
				MatName = (AiScene->mMaterials[_AiMesh->mMaterialIndex]->GetName().C_Str());
		}

		_CurrentSubset->Initialize(_VertexBuffer, _IndexBuffer, VertexDeclaration, VertexSize, NumFaces, NumVerticies);
	};
}
END
#endif // !__STATIC_MESH_H__