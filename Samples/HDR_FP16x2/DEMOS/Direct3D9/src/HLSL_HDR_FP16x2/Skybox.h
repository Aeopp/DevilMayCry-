//-----------------------------------------------------------------------------
// File: skybox.h
//
// Desc: Encapsulation of skybox geometry and textures
//
// Copyright (c) NVIDIA Corporation. All rights reserved.
//-----------------------------------------------------------------------------
#ifndef _SKYBOX_H
#define _SKYBOX_H

#include "nvafx.h"

class Skybox
{
public:
    Skybox();

    HRESULT OnCreateDevice( LPDIRECT3DDEVICE9 pd3dDevice, TCHAR* effectFileName );

    void    Render( LPD3DXMATRIX pmWVP, LPDIRECT3DSURFACE9 RenderTarget0, LPDIRECT3DSURFACE9 RenderTarget1);
	
	void    OnResetDevice( const D3DSURFACE_DESC* pBackBufferSurfaceDesc );
    void    OnLostDevice();
    void    OnDestroyDevice();
	
    void	SetEnvironmentMaps  (	IDirect3DCubeTexture9* pCubeTextureF, 
									IDirect3DCubeTexture9* pCubeTextureA,
									IDirect3DCubeTexture9* pCubeTextureB ) { 
										m_pEnvironmentMapF = pCubeTextureF; 
										m_pEnvironmentMapA = pCubeTextureA;
										m_pEnvironmentMapB = pCubeTextureB;}

protected:
    LPDIRECT3DCUBETEXTURE9          m_pEnvironmentMapF, m_pEnvironmentMapA, m_pEnvironmentMapB;
    ID3DXEffect*                    m_pEffect; 

    LPDIRECT3DVERTEXBUFFER9         m_pVB;
    IDirect3DVertexDeclaration9*    m_pVertexDecl;
    LPDIRECT3DDEVICE9               m_pd3dDevice;   
};

#endif //_SKYBOX_H_