#ifndef __VERTEXS_H__
#define __VERTEXS_H__
#include "EngineDefine.h"
#include "EngineTypedef.h"
#include "AssimpHelper.hpp"
#include "FMath.hpp"

BEGIN(ENGINE)

namespace Vertex
{
	// 노말 매핑 UV 2D
	struct LocationTangentUV
	{
		using VtxType = LocationTangentUV;

		Vector3 Location;
		Vector3 Normal;
		Vector3 Tangent;
		Vector3 BiNormal;
		Vector2 TexCoord;
		static IDirect3DVertexDeclaration9* const
			GetVertexDecl(IDirect3DDevice9* const Device)
		{
			D3DVERTEXELEMENT9 Decl[] =
			{
				{ 0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
				{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,  0 },
				{ 0, 24, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT,  0 },
				{ 0, 36, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL,  0 },
				{ 0, 48, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,  0 },
				D3DDECL_END()
			};
			IDirect3DVertexDeclaration9* VertexDeclaration{ nullptr };
			Device->CreateVertexDeclaration(Decl, &VertexDeclaration);
			return VertexDeclaration;
		};
		static inline VtxType MakeFromAssimpMesh(
			const aiMesh* const AiMesh,
			const uint32 CurrentIdx)
		{
			return VtxType
			{
				FromAssimp(AiMesh->mVertices[CurrentIdx]),
				FromAssimp(AiMesh->mNormals[CurrentIdx]),
				FromAssimp(AiMesh->mTangents[CurrentIdx]),
				FromAssimp(AiMesh->mBitangents[CurrentIdx]),
				FMath::ToVec2(FromAssimp(AiMesh->mTextureCoords[0u][CurrentIdx]))
			};
		};
	};

	struct LocationTangentUVSkinning
	{
		using VtxType = LocationTangentUVSkinning;

		Vector3 Location;
		Vector3 Normal;
		Vector3 Tangent;
		Vector3 BiNormal;
		Vector2 TexCoord;
		Vector4 BoneIds{ 0.0f,0.0f ,0.0f ,0.0f };
		Vector4 Weights{ 0.0f,0.0f,0.0f,0.0f };
		static IDirect3DVertexDeclaration9* const
			GetVertexDecl(IDirect3DDevice9* const Device)
		{
			D3DVERTEXELEMENT9 Decl[] =
			{
				{ 0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
				{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,  0 },
				{ 0, 24, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT,  0 },
				{ 0, 36, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL,  0 },
				{ 0, 48, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,  0 },
				{ 0, 56, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES,  0 },
				{ 0, 72, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT,  0 },
				D3DDECL_END()
			};
			IDirect3DVertexDeclaration9* VertexDeclaration{ nullptr };
			Device->CreateVertexDeclaration(Decl, &VertexDeclaration);
			return VertexDeclaration;
		};
		static inline VtxType MakeFromAssimpMesh(const aiMesh* const AiMesh,
			const uint32 CurrentIdx)
		{
			return VtxType
			{
				FromAssimp(AiMesh->mVertices[CurrentIdx]),
				FromAssimp(AiMesh->mNormals[CurrentIdx]),
				FromAssimp(AiMesh->mTangents[CurrentIdx]),
				FromAssimp(AiMesh->mBitangents[CurrentIdx]),
				FMath::ToVec2(FromAssimp(AiMesh->mTextureCoords[0u][CurrentIdx])) ,
				Vector4{0,0,0,0},
				Vector4 {0,0,0,0}
			};
		};
	};
}
END
#endif // !__VERTEXS_H__