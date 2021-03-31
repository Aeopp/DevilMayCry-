#ifndef __ASSIMP_HELPER_H__
#define __ASSIMP_HELPER_H__
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "EngineDefine.h"
#include "EngineTypedef.h"
#include "Material.h"
#include "VertexInformation.hpp"
#include <filesystem>
#include <iostream>
#include <d3d9.h>
#include <d3dx9.h>
#include <set>
#include "Texture.h"

BEGIN(ENGINE)
namespace AssimpHelper
{
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

	// 열기준 행렬을 전치해 DX의 행기준 행렬로 변환한다.
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

	static inline VertexInformation
		LoadVertexInformation
		(const aiMesh*const AiMesh ,
			IDirect3DDevice9* const Device)
	{
		VertexInformation CurLoadVertexInfo{};
		CurLoadVertexInfo.NumFaces = AiMesh->mNumFaces;
		CurLoadVertexInfo.NumVertices = AiMesh->mNumVertices;
		CurLoadVertexInfo.HasPosition = AiMesh->HasPositions();
		CurLoadVertexInfo.HasNormal          = AiMesh->HasNormals();
		CurLoadVertexInfo.HasTangentBiNormal = AiMesh->HasTangentsAndBitangents();
		// UV 채널이 몇개인가 
		CurLoadVertexInfo.NumUVChannel = AiMesh->GetNumUVChannels();
		CurLoadVertexInfo.NumUVComponents.resize(CurLoadVertexInfo.NumUVChannel);
		CurLoadVertexInfo.VertexSize = 0u;
		std::vector<D3DVERTEXELEMENT9> DeclElements{};

		if (CurLoadVertexInfo.HasPosition)
		{
			DeclElements.push_back
			(
				D3DVERTEXELEMENT9{ 0, 
				static_cast<WORD> ( CurLoadVertexInfo.VertexSize),  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 }
			);
			CurLoadVertexInfo.VertexSize += (sizeof(float) * 3u);
		}
		if (CurLoadVertexInfo.HasNormal)
		{
			DeclElements.push_back
			(
				D3DVERTEXELEMENT9{ 0, 
				static_cast<WORD> (CurLoadVertexInfo.VertexSize), D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 }
			);
			CurLoadVertexInfo.VertexSize += (sizeof(float) * 3u);
		}
		if (CurLoadVertexInfo.HasTangentBiNormal)
		{
			DeclElements.push_back
			(
				D3DVERTEXELEMENT9{ 0, static_cast<WORD> (CurLoadVertexInfo.VertexSize), D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0 }
			);
			CurLoadVertexInfo.VertexSize += (sizeof(float) * 3u);

			DeclElements.push_back
			(
				D3DVERTEXELEMENT9{ 0, static_cast<WORD> (CurLoadVertexInfo.VertexSize), D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL,  0 }
			);

			CurLoadVertexInfo.VertexSize += (sizeof(float) * 3u);
		}

		for (BYTE i = 0u; i < CurLoadVertexInfo.NumUVChannel; ++i)
		{
			CurLoadVertexInfo.NumUVComponents[i] = AiMesh->mNumUVComponents[i];
			D3DDECLTYPE NumDecltypeFloat = D3DDECLTYPE_FLOAT2;
			switch (CurLoadVertexInfo.NumUVComponents[i])
			{
			case 1:
				NumDecltypeFloat = D3DDECLTYPE_FLOAT1;
				break;
			case 2:
				NumDecltypeFloat = D3DDECLTYPE_FLOAT2;
				break;
			case 3:
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
				D3DVERTEXELEMENT9{ 0, static_cast<WORD> (CurLoadVertexInfo.VertexSize),
				static_cast<BYTE>(NumDecltypeFloat), D3DDECLMETHOD_DEFAULT,
				D3DDECLUSAGE_TEXCOORD,  i }
			);
			CurLoadVertexInfo.VertexSize += (sizeof(float) * CurLoadVertexInfo.NumUVComponents[i]);
		}
		DeclElements.push_back(D3DDECL_END());
		IDirect3DVertexDeclaration9* VertexDeclaration{ nullptr };
		Device->CreateVertexDeclaration(DeclElements.data(), &VertexDeclaration);

		// 버텍스 버퍼.
		std::vector<float> Verticies{};

		for (uint32 VerticesIdx = 0u; VerticesIdx <
			CurLoadVertexInfo.NumVertices; ++VerticesIdx)
		{
			if (CurLoadVertexInfo.HasPosition)
			{
				Verticies.push_back(
					AiMesh->mVertices[VerticesIdx].x
				);

				Verticies.push_back(
					AiMesh->mVertices[VerticesIdx].y
				);

				Verticies.push_back(
					AiMesh->mVertices[VerticesIdx].z
				);
				
				CurLoadVertexInfo.LocalVertexLocations->push_back(AssimpHelper::ConvertVec3(AiMesh->mVertices[VerticesIdx]));
			}
			if (CurLoadVertexInfo.HasNormal)
			{
				Verticies.push_back(
					AiMesh->mNormals[VerticesIdx].x
				);

				Verticies.push_back(
					AiMesh->mNormals[VerticesIdx].y
				);

				Verticies.push_back(
					AiMesh->mNormals[VerticesIdx].z
				);
			}
			if (CurLoadVertexInfo.HasTangentBiNormal)
			{
				Verticies.push_back(
					AiMesh->mTangents[VerticesIdx].x
				);

				Verticies.push_back(
					AiMesh->mTangents[VerticesIdx].y
				);

				Verticies.push_back(
					AiMesh->mTangents[VerticesIdx].z
				);


				Verticies.push_back(
					AiMesh->mBitangents[VerticesIdx].x
				);

				Verticies.push_back(
					AiMesh->mBitangents[VerticesIdx].y
				);

				Verticies.push_back(
					AiMesh->mBitangents[VerticesIdx].z
				);
			}
			for (uint32 i = 0u; i < CurLoadVertexInfo.NumUVChannel; ++i)
			{
				const uint32 CurNumUVComponents = CurLoadVertexInfo.NumUVComponents[i];
				const float* TexCoordPtr = reinterpret_cast<const float*>
					(& ( AiMesh->mTextureCoords[i][VerticesIdx] ) );
				for (uint32 j = 0u; j < CurNumUVComponents; ++j)
				{
					Verticies.push_back(TexCoordPtr[j]);
				}
			}
		};
		Device->CreateVertexBuffer(CurLoadVertexInfo.VertexBufferSize,D3DUSAGE_WRITEONLY,NULL,D3DPOOL_MANAGED,&CurLoadVertexInfo.VertexBuffer,nullptr);
		
		void* VertexBufferPtr{ nullptr };
		CurLoadVertexInfo.VertexBuffer->Lock(0, 0,(&VertexBufferPtr), NULL);
		std::memcpy(VertexBufferPtr, Verticies.data(), CurLoadVertexInfo.VertexBufferSize);
		CurLoadVertexInfo.VertexBuffer->Unlock();

		std::vector<uint32> Indicies{};
		for (uint32 FaceIdx = 0u; FaceIdx < CurLoadVertexInfo.NumFaces; ++FaceIdx)
		{
			const aiFace CurrentFace = AiMesh->mFaces[FaceIdx];
			for (uint32 Idx = 0u; Idx < CurrentFace.mNumIndices; ++Idx)
			{
				Indicies.push_back(CurrentFace.mIndices[Idx]);
			}
			const uint32 IdxBufSize = sizeof(uint32) * Indicies.size();
			Device->CreateIndexBuffer(IdxBufSize, D3DUSAGE_WRITEONLY, D3DFMT_INDEX32,
				D3DPOOL_MANAGED, &CurLoadVertexInfo.IndexBuffer, nullptr);
			uint32* IndexBufferPtr{ nullptr };
			CurLoadVertexInfo.IndexBuffer ->Lock(0, 0, reinterpret_cast<void**>(&IndexBufferPtr), NULL);
			std::memcpy(IndexBufferPtr, Indicies.data(), IdxBufSize);
			CurLoadVertexInfo.IndexBuffer->Unlock();
		};

		return CurLoadVertexInfo;
	}
	// Assimp 메쉬로부터 머테리얼 정보 리턴
	static inline Material LoadMaterialFromMesh(
		const aiScene* const AiScene,
		const aiMesh* const  AiMesh,
		const std::set<aiTextureType>& LoadTexTypes,
		// 텍스쳐들이 모여있는 디렉터리 .
		const std::filesystem::path& TexturesDirectory,
		IDirect3DDevice9* const Device )
	{
		Material _CurLoadMaterial{};

		if (AiScene->HasMaterials())
		{
			aiMaterial* AiMaterial = AiScene->mMaterials[AiMesh->mMaterialIndex];

			const std::string
				MatName = (AiMaterial->GetName().C_Str());

			_CurLoadMaterial.Name = MatName;

			for (const auto& LoadTexType:LoadTexTypes)
			{
				const uint32 CurTypeTexCount = AiMaterial->GetTextureCount(LoadTexType);

				if (CurTypeTexCount > 0u)
				{
					_CurLoadMaterial.Textures[LoadTexType].resize(CurTypeTexCount);
					auto& _CurTexTypeMap = _CurLoadMaterial.Textures[LoadTexType];

					for (uint32 TexIdx = 0u; TexIdx < CurTypeTexCount; ++TexIdx)
					{
						aiString Path;
						
						TextureInformation _LoadTexInfo{};
						if (AiMaterial->GetTexture(LoadTexType,
							TexIdx,
							&Path,
							&_LoadTexInfo.MappingType,
							&_LoadTexInfo.UVIdx,
							&_LoadTexInfo.BlendFactor,
							&_LoadTexInfo.TextureOperator,
							&_LoadTexInfo.MappingMode) == AI_SUCCESS)
						{
							const std::filesystem::path FileNamePath = Path.C_Str();
							const std::filesystem::path CurTexFilePath = TexturesDirectory / FileNamePath.filename();

							auto TexPtr = Texture::Create(Device, CurTexFilePath.wstring(), _LoadTexInfo);

							if (TexPtr)
							{
								//_CurTexTypeMap[TexIdx] = std::make_shared<Texture>(TexPtr);
								_CurTexTypeMap[TexIdx].reset(TexPtr, Deleter<Object>());
							};
						}
					};
				}


			}
		}

		return _CurLoadMaterial;
	}

	

};

END


#endif // !__ASSIMP_HELPER_H__