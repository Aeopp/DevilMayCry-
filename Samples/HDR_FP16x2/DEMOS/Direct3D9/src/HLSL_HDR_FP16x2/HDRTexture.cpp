//-----------------------------------------------------------------------------
// File: HDRTexture.cpp
//
// Desc: Class for dealing with HDR textures
//
// Copyright (c) NVIDIA Corporation. All rights reserved.
//-----------------------------------------------------------------------------

#include "nvafx.h"

#include "rgbe.h"
#include "HDRTexture.h"
#include <shared/GetFilePath.h>

#define AUTOMIPMAP_FLAG  0 // NO FP16 AUTOMIPMAP is currently supported in the drivers.  

//-----------------------------------------------------------------------------
// Name: 
//-----------------------------------------------------------------------------
HDRTexture::HDRTexture()
{
	m_data = NULL;
	m_width = m_height = 0;

	m_max_r = m_max_g = m_max_b = 0.0;
	m_min_r = m_min_g = m_min_b = 1e10;
	m_max   = 0;

	m_pCubeMapF = NULL;
	m_pCubeMapA = NULL;
	m_pCubeMapB = NULL;

    m_valid = false;
}

//-----------------------------------------------------------------------------
// Name: 
//-----------------------------------------------------------------------------
HDRTexture::~HDRTexture()
{
	delete [] m_data;
}

//-----------------------------------------------------------------------------
// Name: 
//-----------------------------------------------------------------------------
unsigned char* HDRTexture::GetHDRPixel(int x, int y)
{
  return m_data + ((m_width * (m_height - 1 - y)) + x) * 4;
}
//-----------------------------------------------------------------------------
// Name: 
//-----------------------------------------------------------------------------
void HDRTexture::Analyze()
{
	int i;
	float r, g, b;

	m_max_r = m_max_g = m_max_b = 0.0;
	m_min_r = m_min_g = m_min_b = 1e10;

	unsigned char e;
	unsigned char mine = 255;
	unsigned char maxe = 0;
	unsigned char *ptr = m_data;

	for(i=0; i<m_width*m_height; i++) {
		r = *(ptr + 0);
		g = *(ptr + 1);
		b = *(ptr + 2);
		e = *(ptr + 3);
		if (e < mine) mine = e;
		if (e > maxe) maxe = e;
		
		rgbe2float(&r, &g, &b, ptr);
		if (r > m_max_r) 
			m_max_r = r;
		if (g > m_max_g) 
			m_max_g = g;
		if (b > m_max_b) 
			m_max_b = b;
		if (r < m_min_r)
			m_min_r = r;
		if (g < m_min_g) 
			m_min_g = g;
		if (b < m_min_b) 
			m_min_b = b;

		ptr += 4;
	}

	m_max = m_max_r;
	if (m_max_g > m_max) 
		m_max = m_max_g;
	if (m_max_b > m_max) 
		m_max = m_max_b;
}

//-----------------------------------------------------------------------------
// Name: 
//-----------------------------------------------------------------------------
inline float remap(float x, float max)
{
  if (x > max) x = max;
//  return sqrt(x / max);
  return (x / max);
}


//-----------------------------------------------------------------------------
// Name: 
//-----------------------------------------------------------------------------
bool HDRTexture::LoadTexture(TCHAR* filename)
{
	m_valid = false;

	if(m_data != NULL)
		delete [] m_data;

	FILE *fp = _wfopen( GetFilePath::GetFilePath(filename).c_str(), L"rb");
	if (!fp) {
		fprintf(stderr, "Error opening file '%s'\n", filename);
		return false;
	}

	rgbe_header_info header;
	
	if (RGBE_ReadHeader(fp, &m_width, &m_height, &header))
		return false;

	m_data = new unsigned char[m_width*m_height*4];
	if (!m_data)
		return false;

	if (RGBE_ReadPixels_Raw_RLE(fp, m_data, m_width, m_height))
		return false;

	fclose(fp);

	m_valid = true;

	Analyze();

	return true;
}

//-----------------------------------------------------------------------------
// Name: 
//-----------------------------------------------------------------------------
bool HDRTexture::CreateCubemaps(LPDIRECT3DDEVICE9 pd3dDevice, bool dualTexture)
{
	if(!m_valid) 
		return false;
	
	SAFE_RELEASE( m_pCubeMapF );
	SAFE_RELEASE( m_pCubeMapA );
	SAFE_RELEASE( m_pCubeMapB );

	if(dualTexture){
		pd3dDevice->CreateCubeTexture(m_width / 3, 0, AUTOMIPMAP_FLAG, D3DFMT_G16R16F, D3DPOOL_MANAGED,       &m_pCubeMapA, NULL);
		pd3dDevice->CreateCubeTexture(m_width / 3, 0, AUTOMIPMAP_FLAG, D3DFMT_G16R16F, D3DPOOL_MANAGED,       &m_pCubeMapB, NULL);
		
		FillHalfCubemaps( );
	}
	else{
		pd3dDevice->CreateCubeTexture(m_width / 3, 0, AUTOMIPMAP_FLAG, D3DFMT_A16B16G16R16F, D3DPOOL_MANAGED, &m_pCubeMapF, NULL);

		FillFullCubemap( );
	}

	return true;
}

//-----------------------------------------------------------------------------
// Name: OnDestroyDevice
//-----------------------------------------------------------------------------
void HDRTexture::OnDestroyDevice()
{
    SAFE_RELEASE( m_pCubeMapF );
	SAFE_RELEASE( m_pCubeMapA );
	SAFE_RELEASE( m_pCubeMapB );
}

//-----------------------------------------------------------------------------
// Name: FillFullCubemap
//-----------------------------------------------------------------------------
void HDRTexture::FillFullCubemap(){
	// Fill the surfaces of the cubemap
	// cross is 3 faces wide, 4 faces high
	int face_width  = m_width / 3;
	int face_height = m_height / 4;
 	float *face = new float [face_width * face_height * 4];
	int  pitch = 4 * sizeof(float) * face_width;
    
	// extract 6 faces
	LPDIRECT3DSURFACE9 pFace, pMip1, pMip2;
	RECT               srcRect;

	srcRect.top    = 0;
	srcRect.bottom = face_height;
	srcRect.left   = 0;
	srcRect.right  = face_width;

	float         r, g, b, *ptr;
	unsigned char *src;
	int           xx, yy;

	for(int faceID = D3DCUBEMAP_FACE_POSITIVE_X; faceID <= D3DCUBEMAP_FACE_NEGATIVE_Z; faceID++){
		ptr = face;
		for (int j=0; j<face_height; j++) {
			switch(faceID){
			case D3DCUBEMAP_FACE_POSITIVE_X: 	yy = m_height - (face_height + j + 1); break;
			case D3DCUBEMAP_FACE_NEGATIVE_X: 	yy = m_height - (face_height + j + 1); break;
			case D3DCUBEMAP_FACE_POSITIVE_Y: 	yy = 3 * face_height + j; break;
			case D3DCUBEMAP_FACE_NEGATIVE_Y: 	yy = face_height + j; break;
			case D3DCUBEMAP_FACE_POSITIVE_Z: 	yy = j; break;
			case D3DCUBEMAP_FACE_NEGATIVE_Z: 	yy = m_height - (face_height + j + 1); break;
			}

			for (int i=0; i<face_width; i++) {
				switch(faceID){
				case D3DCUBEMAP_FACE_POSITIVE_X: 	xx = i; break;
				case D3DCUBEMAP_FACE_NEGATIVE_X: 	xx = 2 * face_width + i; break;
				case D3DCUBEMAP_FACE_POSITIVE_Y: 	xx = 2 * face_width - (i + 1); break;
				case D3DCUBEMAP_FACE_NEGATIVE_Y: 	xx = 2 * face_width - (i + 1); break;
				case D3DCUBEMAP_FACE_POSITIVE_Z: 	xx = 2 * face_width - (i + 1); break;
				case D3DCUBEMAP_FACE_NEGATIVE_Z: 	xx = face_width + i; break;
				} 

				src = GetHDRPixel( xx, yy);
				rgbe2float(&r, &g, &b, src);
				*ptr++ = r;
				*ptr++ = g;
				*ptr++ = b;
				*ptr++ = 0.333f*(r+g+b)/m_max;
			}
		}

		m_pCubeMapF->GetCubeMapSurface((D3DCUBEMAP_FACES)faceID, 0, &pFace);
		D3DXLoadSurfaceFromMemory(pFace, NULL, NULL, face, D3DFMT_A32B32G32R32F, pitch, NULL, &srcRect, D3DX_FILTER_LINEAR, 0);
		pFace->Release();

		if(!AUTOMIPMAP_FLAG){
			// Generate the mipmap levels for the face (This could be optimized!)
			for(j = 1; j < m_pCubeMapF->GetLevelCount(); j++){
				m_pCubeMapF->GetCubeMapSurface((D3DCUBEMAP_FACES)faceID, j-1, &pMip1);
				m_pCubeMapF->GetCubeMapSurface((D3DCUBEMAP_FACES)faceID, j, &pMip2);

				D3DXLoadSurfaceFromSurface(pMip2, NULL, NULL, pMip1, NULL, NULL, D3DX_FILTER_LINEAR, 0);

				pMip1->Release();
				pMip2->Release();
			}
		}
	}

	delete [] face;
}

//-----------------------------------------------------------------------------
// Name: FillHalfCubemaps
// Desc: 
//-----------------------------------------------------------------------------
void HDRTexture::FillHalfCubemaps(){
	// Fill the surfaces of the cubemap
	// cross is 3 faces wide, 4 faces high
	int face_width   = m_width / 3;
	int face_height  = m_height / 4;
 	float *faceRG    = new float [face_width * face_height * 2];
	float *faceBA    = new float [face_width * face_height * 2];

	int  pitch = 2 * sizeof(float) * face_width;
    
	// extract 6 faces
	LPDIRECT3DSURFACE9 pFaceRG, pFaceBA, pMip1, pMip2;
	RECT               srcRect;

	srcRect.top    = 0;
	srcRect.bottom = face_height;
	srcRect.left   = 0;
	srcRect.right  = face_width;

	// positive Y
	float *ptrA = faceRG;
	float *ptrB = faceBA;

	float         r, g, b;
	unsigned char *src;
	int           xx, yy;

	for(int faceID = D3DCUBEMAP_FACE_POSITIVE_X; faceID <= D3DCUBEMAP_FACE_NEGATIVE_Z; faceID++){
		ptrA = faceRG;
		ptrB = faceBA;
		for (int j=0; j<face_height; j++) {
			switch(faceID){
			case D3DCUBEMAP_FACE_POSITIVE_X: 	yy = m_height - (face_height + j + 1); break;
			case D3DCUBEMAP_FACE_NEGATIVE_X: 	yy = m_height - (face_height + j + 1); break;
			case D3DCUBEMAP_FACE_POSITIVE_Y: 	yy = 3 * face_height + j; break;
			case D3DCUBEMAP_FACE_NEGATIVE_Y: 	yy = face_height + j; break;
			case D3DCUBEMAP_FACE_POSITIVE_Z: 	yy = j; break;
			case D3DCUBEMAP_FACE_NEGATIVE_Z: 	yy = m_height - (face_height + j + 1); break;
			}

			for (int i=0; i<face_width; i++) {
				switch(faceID){
				case D3DCUBEMAP_FACE_POSITIVE_X: 	xx = i; break;
				case D3DCUBEMAP_FACE_NEGATIVE_X: 	xx = 2 * face_width + i; break;
				case D3DCUBEMAP_FACE_POSITIVE_Y: 	xx = 2 * face_width - (i + 1); break;
				case D3DCUBEMAP_FACE_NEGATIVE_Y: 	xx = 2 * face_width - (i + 1); break;
				case D3DCUBEMAP_FACE_POSITIVE_Z: 	xx = 2 * face_width - (i + 1); break;
				case D3DCUBEMAP_FACE_NEGATIVE_Z: 	xx = face_width + i; break;
				} 

				src = GetHDRPixel( xx, yy);
				rgbe2float(&r, &g, &b, src);
				*ptrA++ = r;
				*ptrA++ = g;
				*ptrB++ = b;
				*ptrB++ = 0.333f*(r+g+b)/m_max;
			}
		}

		m_pCubeMapA->GetCubeMapSurface((D3DCUBEMAP_FACES)faceID, 0, &pFaceRG);
		m_pCubeMapB->GetCubeMapSurface((D3DCUBEMAP_FACES)faceID, 0, &pFaceBA);
		D3DXLoadSurfaceFromMemory(pFaceRG, NULL, NULL, faceRG, D3DFMT_G32R32F, pitch, NULL, &srcRect, D3DX_FILTER_LINEAR, 0);
		D3DXLoadSurfaceFromMemory(pFaceBA, NULL, NULL, faceBA, D3DFMT_G32R32F, pitch, NULL, &srcRect, D3DX_FILTER_LINEAR, 0);
		pFaceRG->Release();
		pFaceBA->Release();

		if(!AUTOMIPMAP_FLAG){
			// Generate the mipmap levels for the face in channel A (This could be optimized!)
			for(j = 1; j < m_pCubeMapA->GetLevelCount(); j++){
				m_pCubeMapA->GetCubeMapSurface((D3DCUBEMAP_FACES)faceID, j-1, &pMip1);
				m_pCubeMapA->GetCubeMapSurface((D3DCUBEMAP_FACES)faceID, j, &pMip2);

				D3DXLoadSurfaceFromSurface(pMip2, NULL, NULL, pMip1, NULL, NULL, D3DX_FILTER_LINEAR, 0);

				pMip1->Release();
				pMip2->Release();
			}

			// Generate the mipmap levels for the face in channel B (This could be optimized!)
			for(j = 1; j < m_pCubeMapB->GetLevelCount(); j++){
				m_pCubeMapB->GetCubeMapSurface((D3DCUBEMAP_FACES)faceID, j-1, &pMip1);
				m_pCubeMapB->GetCubeMapSurface((D3DCUBEMAP_FACES)faceID, j, &pMip2);

				D3DXLoadSurfaceFromSurface(pMip2, NULL, NULL, pMip1, NULL, NULL, D3DX_FILTER_LINEAR, 0);

				pMip1->Release();
				pMip2->Release();
			}
		}
	}

	delete [] faceRG;
	delete [] faceBA;
}

