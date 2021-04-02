#pragma once

#include "nvafx.h"
//#include "HDR_FP16x2.h"
#include <math.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "skybox.h"
#include "model3D.h"
#include "HDRTexture.h"

//--------------------------------------------------------------------------------------
// Texture coordinate rectangle
struct CoordRect
{
    float fLeftU,  fTopV;
    float fRightU, fBottomV;
};

// Vertex structure for different postprocessing effects
struct ScreenVertex0
{
    D3DXVECTOR4 p; // position
};

struct ScreenVertex1
{
    D3DXVECTOR4 p; // position
    D3DXVECTOR2 t; // texture coordinate
};

struct ScreenVertex4
{
    D3DXVECTOR4 p;    // position
    D3DXVECTOR4 t[2]; // texture coordinate
};

struct ScreenVertex8
{
    D3DXVECTOR4 p;    // position
    D3DXVECTOR4 t[4]; // texture coordinate
};

static const DWORD MyFVF0 = D3DFVF_XYZW;//   | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE2(0);
static const DWORD MyFVF1 = D3DFVF_XYZRHW | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE2(0);
static const DWORD MyFVF4 = D3DFVF_XYZRHW | D3DFVF_TEX2 | D3DFVF_TEXCOORDSIZE4(0) | D3DFVF_TEXCOORDSIZE4(1); 
static const DWORD MyFVF8 = D3DFVF_XYZRHW |	D3DFVF_TEX4 | D3DFVF_TEXCOORDSIZE4(0) | D3DFVF_TEXCOORDSIZE4(1) | D3DFVF_TEXCOORDSIZE4(2) | D3DFVF_TEXCOORDSIZE4(3);

#define MAX_SAMPLES   16       // Maximum number of texture grabs
#define MAX_LUM_TEX   3        // 64x64, 16x16, 4x4.

//--------------------------------------------------------------------------------------
class HDREngine{
public:
	HDREngine();

	bool isDeviceAcceptable(IDirect3D9* pD3D, D3DCAPS9* pCaps, D3DFORMAT AdapterFormat);

	HRESULT onCreateDevice (IDirect3DDevice9* pd3dDevice);
	HRESULT onResetDevice  (IDirect3DDevice9* pd3dDevice,  const D3DSURFACE_DESC* pBackBufferSurfaceDesc, bool dualTexture, bool anisoDecimation);
	HRESULT onDestroyDevice();
	HRESULT onLostDevice   ();
	HRESULT onFrameRender  (const D3DXMATRIX *viewMatrix, const D3DXMATRIX *projMatrix, const D3DXMATRIX *worldMatrix, const D3DXVECTOR3 *eyePosition, const float frameDelta);

	void registerAssets(HDRTexture* envMap, HDRTexture* diffMap, Skybox* skybox, Model3D* model);
	void resizeScreen(int newWidth, int newHeight);
	
	//void useAnisoDecimation( bool flag  ){ m_bUseAnisoDecimation = flag;  };
	void setExposure       ( float value){ m_fExposure       = value;     };
	
private:
	// Main render functions
	void RenderGeometry     (bool useMRT, LPDIRECT3DSURFACE9 RenderTarget0, LPDIRECT3DSURFACE9 RenderTarget1 = NULL);
	void calculateLuminance (const float frameDelta, LPDIRECT3DTEXTURE9 channelA, LPDIRECT3DTEXTURE9 channelB = NULL);
	void ToneMapping        (LPDIRECT3DTEXTURE9 channelA, LPDIRECT3DTEXTURE9 channelB = NULL);
	void glowPass           (LPDIRECT3DTEXTURE9 channelA, LPDIRECT3DTEXTURE9 channelB = NULL);
	void DownscaleScene     (LPDIRECT3DSURFACE9 RT0, LPDIRECT3DTEXTURE9 channelA);
	void DownscaleSceneAniso(LPDIRECT3DSURFACE9 RT0, LPDIRECT3DSURFACE9 RT_int, LPDIRECT3DTEXTURE9 channelA, LPDIRECT3DTEXTURE9 Tex_int);
	void LuminancePass      (LPDIRECT3DSURFACE9 RT0, int method, ScreenVertex4 *vertArray, LPDIRECT3DTEXTURE9 channelA,  LPDIRECT3DTEXTURE9 channelB = NULL);
	void LuminanceAdaptation(LPDIRECT3DSURFACE9 dst, LPDIRECT3DTEXTURE9 src, LPDIRECT3DTEXTURE9 src2, const float frameDelta);
	void BlurTarget         (LPDIRECT3DSURFACE9 RT0, bool verticalDir, LPDIRECT3DTEXTURE9 channelA);
	
	// Render helpers
	void DrawFullScreenQuad(void *scoords, DWORD MyFVF, unsigned int size);

	// Offest calculation functions
	void CalculateOffsetsAndQuadCoords();
	void ApplyTexelToPixelCorrection(ScreenVertex0 dst[], int srcW, int srcH, CoordRect *coords);
	void ApplyTexelToPixelCorrection(ScreenVertex1 dst[], int srcW, int srcH, CoordRect *coords);
	void ApplyTexelToPixelCorrection(ScreenVertex4 dst[], int srcW, int srcH, CoordRect *coords, D3DXVECTOR2 offsets[]);
	void ApplyTexelToPixelCorrection(ScreenVertex8 dst[], int srcW, int srcH, CoordRect *coords, D3DXVECTOR2 offsets[]);

	void CalculateSampleOffset_4x4Bilinear( DWORD dwWidth, DWORD dwHeight, D3DXVECTOR2 avSampleOffsets[] );
	void CalculateOffsets_GaussianBilinear( float texSize, float *coordOffset, float *gaussWeight, int kernelRadius );

private:
	// Flags	
	bool					m_bUse2xFp;
	bool					m_bUseVTF;
	bool					m_bUseAnisoDecimation;
	bool                    m_bSelfAdaptation;
	float                   m_fExposure;
	int                     m_iPreviousLuminance;

	// Buffer sizes
	int                     m_iScreenWidth;
	int                     m_iScreenHeight;
	int                     m_iCroppedWidth;
	int                     m_iCroppedHeight;
	int                     m_iShrinkWidth;
	int                     m_iShrinkHeight;

	// Render targets and associated surfaces for 1xFP16 path
	LPDIRECT3DSURFACE9		m_pSceneFSurf; 
	LPDIRECT3DSURFACE9		m_pfp16ShrinkFSurf;
	LPDIRECT3DSURFACE9		m_pfp16ShrinkFSurf_1;
	LPDIRECT3DSURFACE9		m_pfp16ShrinkFSurf_2;

	LPDIRECT3DTEXTURE9		m_pSceneF; 
	LPDIRECT3DTEXTURE9		m_pfp16ShrinkF;
	LPDIRECT3DTEXTURE9		m_pfp16ShrinkF_1;
	LPDIRECT3DTEXTURE9		m_pfp16ShrinkF_2;

	// Render targets and associated surfaces for 2xFP16 path
	LPDIRECT3DSURFACE9		m_pSceneASurf,         m_pSceneBSurf; 
	LPDIRECT3DSURFACE9		m_pfp16ShrinkASurf,    m_pfp16ShrinkBSurf;
	LPDIRECT3DSURFACE9		m_pfp16ShrinkASurf_1,  m_pfp16ShrinkASurf_2;

	LPDIRECT3DTEXTURE9		m_pSceneA,         m_pSceneB; 
	LPDIRECT3DTEXTURE9		m_pfp16ShrinkA,    m_pfp16ShrinkB;
	LPDIRECT3DTEXTURE9		m_pfp16ShrinkA_1,  m_pfp16ShrinkA_2;

	// Render targets and associated surfaces for luminance self-adaptation
	LPDIRECT3DTEXTURE9      m_pfp16Lum[MAX_LUM_TEX];
	LPDIRECT3DTEXTURE9      m_pfp32Lum[3];                  // The 1x1 texture to use VTF for the luminance
  
	LPDIRECT3DSURFACE9      m_pfp16LumSurf[MAX_LUM_TEX];
	LPDIRECT3DSURFACE9      m_pfp32LumSurf[3];   

	// FX
	LPD3DXEFFECT			m_pHDREffect;

	// D3D Device  
	LPDIRECT3DDEVICE9       m_pD3DDevice;
	LPDIRECT3DSURFACE9		m_pBackBuffer; 

	// Camera info
	const D3DXMATRIX*       m_pViewMatrix;
	const D3DXMATRIX*       m_pProjMatrix; 
	const D3DXMATRIX*       m_pWorldMatrix;
	const D3DXVECTOR3*      m_pEyePosition;

	// Art assets
	HDRTexture*             m_pHDREnvMap;
	HDRTexture*             m_pHDRDifMap;
	Skybox*                 m_pSkybox;
	Model3D*                m_pModel3D;

    // Global variables for the sampling on downscale and blurring
	float                   m_gaussWeights[MAX_SAMPLES];

	ScreenVertex0           m_fullscreenVTFTonemapPass[3];
	ScreenVertex1           m_fullscreenQuadCoords[3];
    ScreenVertex4           m_downsampleQuadCoords[3];
	ScreenVertex1           m_downsampleAnisoQuadCoords[2][3];
	ScreenVertex4           m_luminanceQuadCoords[MAX_LUM_TEX + 1][3];
	ScreenVertex8           m_blurQuadCoordsV[3];
	ScreenVertex8           m_blurQuadCoordsH[3];
};



