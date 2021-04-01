//-----------------------------------------------------------------------------
// File: Model3D.cpp
//
// Desc: Encapsulation of Model3D geometry and textures
//
// Copyright (c) NVIDIA Corporation. All rights reserved.
//-----------------------------------------------------------------------------

#include "model3D.h"

#include <shared/GetFilePath.h>

struct MODEL3D_VERTEX
{
    D3DXVECTOR4 pos;
	D3DXVECTOR3 nor;
    D3DXVECTOR3 tex;
};


D3DVERTEXELEMENT9 g_aModel3DDecl[] =
{
    {0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
	{0, 20, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
	D3DDECL_END()
};


//-----------------------------------------------------------------------------
// Name: Model3D
// Desc: Constructor
//-----------------------------------------------------------------------------
Model3D::Model3D()
{
    m_pd3dDevice        = NULL;
    m_pEffect           = NULL;

	m_pEnvironmentMapF = NULL;
	m_pEnvironmentMapA = NULL;
	m_pEnvironmentMapB = NULL;

	m_pDiffuseMapF     = NULL;
	m_pDiffuseMapA     = NULL;
	m_pDiffuseMapB     = NULL;

	Reflective = true;
}

Model3D::~Model3D()
{
  
}

//-----------------------------------------------------------------------------
// Name: OnCreateDevice
// Desc: 
//-----------------------------------------------------------------------------
HRESULT Model3D::OnCreateDevice( LPDIRECT3DDEVICE9 pd3dDevice,  TCHAR* meshFileName, TCHAR* effectFileName )
{
	m_pd3dDevice       = pd3dDevice;
	

	D3DXCreateEffectFromFile(pd3dDevice,	GetFilePath::GetFilePath(effectFileName, true).c_str(),
											NULL,
											NULL,
											D3DXSHADER_DEBUG,
											NULL,
											&m_pEffect,
											NULL);
	// Load the Mesh
	D3DXLoadMeshFromX(GetFilePath::GetFilePath(meshFileName, true).c_str(),
								D3DXMESH_MANAGED,
								pd3dDevice,
								NULL,
								NULL,
								NULL,
								NULL,
								&m_pMesh);

	// Make sure the normals are calculated.
	ID3DXMesh* pTempMesh = NULL;
	m_pMesh->CloneMesh(D3DXMESH_MANAGED, g_aModel3DDecl, pd3dDevice, &pTempMesh);
	SAFE_RELEASE(m_pMesh);
	D3DXComputeNormals(pTempMesh, NULL);
	m_pMesh = pTempMesh;

	return S_OK;
}



//-----------------------------------------------------------------------------
// Name: Render
// Desc: 
//-----------------------------------------------------------------------------
void Model3D::Render( const D3DXMATRIX* pmW, const D3DXMATRIX* pmWVP, const D3DXVECTOR3* pos, bool MRTRender )
{
    if(m_pEffect == NULL)
		return;
	
	m_pEffect->SetMatrix( "WorldViewProjection",   pmWVP );
	m_pEffect->SetMatrix( "World",                 pmW   );
	m_pEffect->SetValue ( "EyePosition", pos, sizeof(D3DXVECTOR3));
        
    // Draw the Model3D
    UINT iPass, iPasses;
   
	// Choose right technique
	if(MRTRender){
		if(Reflective)
			m_pEffect->SetTechnique( "RenderModel_2RT_reflective" );
		else
			m_pEffect->SetTechnique( "RenderModel_2RT_refractive" );

		// Set Textures
		m_pEffect->SetTexture( "EnvMapA", m_pEnvironmentMapA );
		m_pEffect->SetTexture( "EnvMapB", m_pEnvironmentMapB );
		m_pEffect->SetTexture( "DifMapA", m_pDiffuseMapA );
		m_pEffect->SetTexture( "DifMapB", m_pDiffuseMapB );
		
	}
	else{
		if(Reflective)
			m_pEffect->SetTechnique( "RenderModel_reflective" );
		else
			m_pEffect->SetTechnique( "RenderModel_refractive" );

		// Set Textures
		m_pEffect->SetTexture( "EnvMapF", m_pEnvironmentMapF );
		m_pEffect->SetTexture( "DifMapF", m_pDiffuseMapF );
	}

    m_pEffect->Begin( &iPasses, 0 );
    for( iPass=0; iPass < iPasses; iPass++ )
    {
        m_pEffect->BeginPass(iPass);      
		m_pMesh->DrawSubset(0);
        m_pEffect->EndPass();
    }
    m_pEffect->End();
}

//-----------------------------------------------------------------------------
// Name: OnLostDevice
//-----------------------------------------------------------------------------
void Model3D::OnLostDevice()
{
    if( m_pEffect )
        m_pEffect->OnLostDevice();
}

//-----------------------------------------------------------------------------
// Name: OnResetDevice
//-----------------------------------------------------------------------------
void Model3D::OnResetDevice()
{
    if( m_pEffect )
        m_pEffect->OnResetDevice();
}
//-----------------------------------------------------------------------------
// Name: OnDestroyDevice
//-----------------------------------------------------------------------------
void Model3D::OnDestroyDevice()
{
    m_pd3dDevice       = NULL;
	m_pEnvironmentMapF = NULL;
	m_pEnvironmentMapA = NULL;
	m_pEnvironmentMapB = NULL;

	m_pDiffuseMapF     = NULL;
	m_pDiffuseMapA     = NULL;
	m_pDiffuseMapB     = NULL;
    
	SAFE_RELEASE( m_pEffect );
	SAFE_RELEASE( m_pMesh );
}


