//-----------------------------------------------------------------------------
// File: skybox.cpp
//
// Desc: Encapsulation of skybox geometry and textures
//
// Copyright (c) NVIDIA Corporation. All rights reserved.
//-----------------------------------------------------------------------------
#include "nvafx.h"
#include "skybox.h"

#include <shared/GetFilePath.h>

struct SKYBOX_VERTEX
{
    D3DXVECTOR4 pos;
    D3DXVECTOR3 tex;
};


D3DVERTEXELEMENT9 g_aSkyboxDecl[] =
{
    { 0, 0,  D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
    D3DDECL_END()
};


//-----------------------------------------------------------------------------
// Name: Skybox
// Desc: Constructor
//-----------------------------------------------------------------------------
Skybox::Skybox()
{
    m_pVB               = NULL;
    m_pd3dDevice        = NULL;
    m_pEffect           = NULL;
    m_pVertexDecl       = NULL;

	m_pEnvironmentMapF = NULL;
	m_pEnvironmentMapA = NULL;
	m_pEnvironmentMapB = NULL;
}


//-----------------------------------------------------------------------------
// Name: OnCreateDevice
// Desc: 
//-----------------------------------------------------------------------------
HRESULT Skybox::OnCreateDevice( LPDIRECT3DDEVICE9 pd3dDevice,   TCHAR* effectFileName )
{
    m_pd3dDevice      = pd3dDevice;
    
    // Load the Effects file
    D3DXCreateEffectFromFile(pd3dDevice,	GetFilePath::GetFilePath(effectFileName, true).c_str(),
											NULL, 
											NULL, 
											D3DXSHADER_DEBUG, 
											NULL, 
											&m_pEffect, 
											NULL );

    // Create vertex declaration
    pd3dDevice->CreateVertexDeclaration( g_aSkyboxDecl, &m_pVertexDecl );

    return S_OK;
}


//-----------------------------------------------------------------------------
// Name: OnCreateDevice
// Desc: 
//-----------------------------------------------------------------------------
void Skybox::OnResetDevice( const D3DSURFACE_DESC* pBackBufferSurfaceDesc )
{
    HRESULT hr;

    if(m_pd3dDevice == NULL)
		return;

    if( m_pEffect )
        m_pEffect->OnResetDevice();

    m_pd3dDevice->CreateVertexBuffer( 4 * sizeof(SKYBOX_VERTEX), 
                                         D3DUSAGE_WRITEONLY, 0, 
                                         D3DPOOL_DEFAULT, &m_pVB, NULL );

    // Fill the vertex buffer
    SKYBOX_VERTEX* pVertex = NULL;
    V( m_pVB->Lock( 0, 0, (void**)&pVertex, 0 ) );

    // Map texels to pixels 
    float fHighW = -1.0f - (1.0f/(float)pBackBufferSurfaceDesc->Width);
    float fHighH = -1.0f - (1.0f/(float)pBackBufferSurfaceDesc->Height);
    float fLowW  = 1.0f + (1.0f/(float)pBackBufferSurfaceDesc->Width);
    float fLowH  = 1.0f + (1.0f/(float)pBackBufferSurfaceDesc->Height);

    pVertex[0].pos = D3DXVECTOR4(fLowW, fLowH, 1.0f, 1.0f);
    pVertex[1].pos = D3DXVECTOR4(fLowW, fHighH, 1.0f, 1.0f);
    pVertex[2].pos = D3DXVECTOR4(fHighW, fLowH, 1.0f, 1.0f);
    pVertex[3].pos = D3DXVECTOR4(fHighW, fHighH, 1.0f, 1.0f);

    m_pVB->Unlock();
}


//-----------------------------------------------------------------------------
// Name: Render
// Desc: 
//-----------------------------------------------------------------------------
void Skybox::Render( LPD3DXMATRIX pmWVP, LPDIRECT3DSURFACE9 RenderTarget0, LPDIRECT3DSURFACE9 RenderTarget1)
{
    UINT iPass, iPasses;
	
	if( m_pEffect == NULL )
		return;

	D3DXMATRIX IWVP; D3DXMatrixInverse  ( &IWVP, NULL, pmWVP );

	// Set transformation 
	m_pEffect->SetMatrix( "InvWorldViewProjection", &IWVP );
    m_pEffect->SetTechnique( "RenderSkybox" );   
    
	m_pd3dDevice->SetStreamSource( 0, m_pVB, 0, sizeof(SKYBOX_VERTEX) );
    m_pd3dDevice->SetVertexDeclaration( m_pVertexDecl );
	
	if(RenderTarget1 != NULL){
		m_pEffect->SetTexture( "EnvMap", m_pEnvironmentMapA );
		// Draw the skybox 
		m_pEffect->Begin( &iPasses, 0 );
		for( iPass=0; iPass < iPasses; iPass++ )
		{
			m_pEffect->BeginPass( iPass );   

			m_pd3dDevice->SetRenderTarget(0, RenderTarget0);
			m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );

			m_pEffect->SetTexture( "EnvMap", m_pEnvironmentMapB );
			m_pEffect->CommitChanges();

			m_pd3dDevice->SetRenderTarget(0, RenderTarget1);
			m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
			
			m_pEffect->EndPass();
		}
		m_pEffect->End();
	}
	else{
		m_pEffect->SetTexture( "EnvMap", m_pEnvironmentMapF );
		m_pEffect->Begin( &iPasses, 0 );
		for( iPass=0; iPass < iPasses; iPass++ )
		{
			m_pEffect->BeginPass( iPass );   

			m_pd3dDevice->SetRenderTarget(0, RenderTarget0);
			m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );

			m_pEffect->EndPass();
		}
		m_pEffect->End();
	}
}


//-----------------------------------------------------------------------------
// Name: OnCreateDevice
// Desc: 
//-----------------------------------------------------------------------------
void Skybox::OnLostDevice()
{
    HRESULT hr;
    if( m_pEffect )
        V( m_pEffect->OnLostDevice() );
    SAFE_RELEASE( m_pVB );
}


//-----------------------------------------------------------------------------
// Name: OnCreateDevice
// Desc: 
//-----------------------------------------------------------------------------
void Skybox::OnDestroyDevice()
{
    m_pd3dDevice       = NULL;

    m_pEnvironmentMapF = NULL;
	m_pEnvironmentMapA = NULL;
	m_pEnvironmentMapB = NULL;

    SAFE_RELEASE( m_pEffect );
    SAFE_RELEASE( m_pVertexDecl );
	SAFE_RELEASE( m_pVB );
}




