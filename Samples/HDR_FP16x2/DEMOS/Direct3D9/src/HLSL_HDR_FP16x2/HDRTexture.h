//-----------------------------------------------------------------------------
// File: HDRTexture.h
//
// Desc: Class for dealing with HDR textures
//
// Copyright (c) NVIDIA Corporation. All rights reserved.
//-----------------------------------------------------------------------------

#ifndef _HDRTEXTURE_H
#define _HDRTEXTURE_H

#include "nvafx.h"

class HDRTexture {
public:
	HDRTexture();
	~HDRTexture();

	unsigned char *GetHDRPixel(int x, int y);
	
	bool LoadTexture(TCHAR* filename);

	bool CreateCubemaps(LPDIRECT3DDEVICE9 pd3dDevice, bool dualTexture);

	IDirect3DCubeTexture9* GetEnvironmentMap() { return m_pCubeMapF; }

	int GetWidth()  { return m_width; };
	int GetHeight() { return m_height; };

	void    OnDestroyDevice();
	
	LPDIRECT3DCUBETEXTURE9 m_pCubeMapF;
	LPDIRECT3DCUBETEXTURE9 m_pCubeMapA;
	LPDIRECT3DCUBETEXTURE9 m_pCubeMapB;

private:
	void Analyze();
	void FillFullCubemap( );
	void FillHalfCubemaps( );
private:

	

	bool m_valid;
	int m_width, m_height;
	unsigned char *m_data;

	float m_max_r, m_max_g, m_max_b;
	float m_min_r, m_min_g, m_min_b;
	float m_max;
};

#endif
