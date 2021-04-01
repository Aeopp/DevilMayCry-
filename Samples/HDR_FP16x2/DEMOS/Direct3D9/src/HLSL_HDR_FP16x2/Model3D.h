//-----------------------------------------------------------------------------
// File: Model3D.h
//
// Desc: Encapsulation of Model3D geometry and textures
//
// Copyright (c) NVIDIA Corporation. All rights reserved.
//-----------------------------------------------------------------------------
#ifndef _MODEL3D_H
#define _MODEL3D_H

#include "nvafx.h"

class Model3D
{
public:
	Model3D();
	~Model3D();

    HRESULT OnCreateDevice( LPDIRECT3DDEVICE9 pd3dDevice,   TCHAR* meshFileName, 
															TCHAR* effectFileName );
    
    void    Render( const D3DXMATRIX* pmW, const D3DXMATRIX* pmWVP, const D3DXVECTOR3* pos, bool MRTRender);
    void    OnLostDevice();
	void    OnResetDevice();
    void    OnDestroyDevice();

	void Model3D::SetReflectiveMode(bool flag){
		Reflective = flag;
	}

	void	SetEnvironmentMaps  (	IDirect3DCubeTexture9* pCubeTextureF, 
									IDirect3DCubeTexture9* pCubeTextureA,
									IDirect3DCubeTexture9* pCubeTextureB ) { 
										m_pEnvironmentMapF = pCubeTextureF; 
										m_pEnvironmentMapA = pCubeTextureA;
										m_pEnvironmentMapB = pCubeTextureB;}

	void	SetDiffuseMaps      (	IDirect3DCubeTexture9* pCubeTextureF, 
									IDirect3DCubeTexture9* pCubeTextureA,
									IDirect3DCubeTexture9* pCubeTextureB ) { 
										m_pDiffuseMapF = pCubeTextureF; 
										m_pDiffuseMapA = pCubeTextureA;
										m_pDiffuseMapB = pCubeTextureB;}

protected:
	LPD3DXMESH						m_pMesh;
    LPDIRECT3DCUBETEXTURE9          m_pEnvironmentMapF, m_pEnvironmentMapA, m_pEnvironmentMapB;
	LPDIRECT3DCUBETEXTURE9          m_pDiffuseMapF, m_pDiffuseMapA, m_pDiffuseMapB;
	LPD3DXEFFECT			        m_pEffect;

    LPDIRECT3DDEVICE9               m_pd3dDevice;   

	bool Reflective;
};

#endif //_MODEL3D_H_