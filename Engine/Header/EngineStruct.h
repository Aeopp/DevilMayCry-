#ifndef __ENGINE_STRUCT_H__
#define __ENGINE_STRUCT_H__
BEGIN(ENGINE)
typedef struct tagVertexBufferDesc
{
	uint32							nBufferSize;
	uint32							nStride;
	uint32							nNumFaces;
	uint32							nNumVertices;
	uint32							nNumUVChannel;
	
	std::vector<uint32>				vecNumUVComponents;

	bool							bHasPosition;
	bool							bHasNormal;
	bool							bHasTangentBiNormal;

	std::shared_ptr<std::vector<D3DXVECTOR3>> LocalVertexLocation;

	LPDIRECT3DVERTEXDECLARATION9	pVertexDecl;
public:
	tagVertexBufferDesc()
	{
		nBufferSize = nStride = nNumFaces = nNumVertices = nNumUVChannel = 0u;
		pVertexDecl = nullptr;
		bHasPosition = bHasNormal = bHasTangentBiNormal = false;
		LocalVertexLocation = std::make_shared<std::vector<D3DXVECTOR3>>();
	}
	~tagVertexBufferDesc()
	{
		SafeRelease(pVertexDecl);
	}
}VERTEXBUFFERDESC, *LPVERTEXBUFFERDESC;

typedef struct tagMaterial
{
public:
	typedef std::unordered_map<UINT, std::vector<std::shared_ptr<class Texture>>> TEXTURES;
	TEXTURES		Textures;
	TSTRING			sName;
	D3DMATERIAL9	tMaterial;
public:
	tagMaterial()
	{
		sName = TEXT("");
		ZeroMemory(&tMaterial, sizeof(D3DMATERIAL9));
	}
	const TSTRING& GetMaterialName() const
	{
		return sName;
	}
	std::shared_ptr<Texture> GetTexture(const UINT TextureType, const uint64 TextureIndex)const
	{
		if (auto TexTypeIter = Textures.find(TextureType);
			TexTypeIter!= std::end  ( Textures ))
		{
			auto& TexArray = TexTypeIter->second;
			if (TextureIndex < TexArray.size())
			{
				auto SpTexture= TexArray[TextureIndex];   
				if (SpTexture)
				{
					return SpTexture;
				}
			}
			
			return nullptr;
		}
	}
}MATERIAL, *LPMATERIAL;

typedef struct tagTextureDesc
{
	uint32	nUVIdx;
	float	fBlendFactor;
	uint32	eTextureOperator;
	uint32	eMappingMode;
	uint32	eMappingType;
}TEXTUREDESC, * LPTEXTUREDESC;
END
#endif // !__ENGINE_STRUCT_H__