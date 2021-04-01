//--------------------------------------------------------------------------------------
//
//
//
//
//
//
//
//
//
//
//--------------------------------------------------------------------------------------

#include "HDREngine.h"

#include <shared/GetFilePath.h>

#define LUMINANCE_DOWNSAMPLE_LOG    0
#define LUMINANCE_DOWNSAMPLE        1
#define LUMINANCE_DOWNSAMPLE_EXP    2

//--------------------------------------------------------------------------------------
//
//
//--------------------------------------------------------------------------------------
HDREngine::HDREngine(){
	// Initialize varables
	m_bUseVTF               = true;
	m_bUse2xFp              = true;
	m_bUseAnisoDecimation   = false;
	m_fExposure             = 1.0f;
	m_iPreviousLuminance    = 1;
	
	m_iScreenWidth   = 0;
	m_iScreenHeight  = 0;
	m_iCroppedWidth  = 0;
	m_iCroppedHeight = 0;
	m_iShrinkWidth   = 0;
	m_iShrinkHeight  = 0;
	
	//Initialize surfaces
	m_pSceneASurf        = NULL;
	m_pSceneBSurf        = NULL;
	m_pSceneFSurf        = NULL;
	m_pfp16ShrinkASurf   = NULL;
	m_pfp16ShrinkBSurf   = NULL;
	m_pfp16ShrinkFSurf   = NULL;
	m_pfp16ShrinkASurf_1 = NULL;
	m_pfp16ShrinkASurf_2 = NULL;
	m_pfp16ShrinkFSurf_1 = NULL;
	m_pfp16ShrinkFSurf_2 = NULL;
	m_pfp32LumSurf[0]    = NULL;      
	m_pfp32LumSurf[1]    = NULL;   
	m_pfp32LumSurf[2]    = NULL;  
	
	//Initialize textures
	m_pSceneA        = NULL;
	m_pSceneB        = NULL;
	m_pSceneF        = NULL;
	m_pfp16ShrinkA   = NULL;
	m_pfp16ShrinkB   = NULL;
	m_pfp16ShrinkF   = NULL;
	m_pfp16ShrinkA_1 = NULL;
	m_pfp16ShrinkA_2 = NULL;
	m_pfp16ShrinkF_1 = NULL;
	m_pfp16ShrinkF_2 = NULL;
	m_pfp32Lum[0]    = NULL;     
	m_pfp32Lum[1]    = NULL;   
	m_pfp32Lum[2]    = NULL;
	
	for(int i = 0; i < MAX_LUM_TEX; i++){
		m_pfp16LumSurf[i] = NULL;
		m_pfp16Lum[i] = NULL;
	}

	//Initialize effects
	m_pHDREffect        = NULL;

	// Initialize D3D device
	m_pD3DDevice        = NULL;

	// Initialize supporting objects.
	m_pHDREnvMap        = NULL;
	m_pHDRDifMap        = NULL;
	m_pSkybox           = NULL;
	m_pModel3D          = NULL;
}

//--------------------------------------------------------------------------------------
// Called during device initialization, this code checks the device for some 
// minimum set of capabilities, and rejects those that don't pass by returning false.
//--------------------------------------------------------------------------------------
bool HDREngine::isDeviceAcceptable( IDirect3D9* pD3D, D3DCAPS9* pCaps, D3DFORMAT AdapterFormat)
{
 	if( pCaps->VertexShaderVersion < D3DVS_VERSION(2, 0)) 
		return false;

	if( pCaps->PixelShaderVersion < D3DPS_VERSION(2, 0))
		return false;

    // Support for D3DFMT_A16B16G16R16F render target
    if( FAILED( pD3D->CheckDeviceFormat( pCaps->AdapterOrdinal, pCaps->DeviceType,
                    AdapterFormat, D3DUSAGE_RENDERTARGET, 
                    D3DRTYPE_TEXTURE, D3DFMT_A16B16G16R16F) ) )
    {
        return false;
    }

	// Support for D3DFMT_G16R16F render target
    if( FAILED( pD3D->CheckDeviceFormat( pCaps->AdapterOrdinal, pCaps->DeviceType,
                    AdapterFormat, D3DUSAGE_RENDERTARGET, 
                    D3DRTYPE_TEXTURE, D3DFMT_G16R16F) ) )
    {
        return false;
    }

	// Support for D3DFMT_A16B16G16R16F cubemaps 
    if( pD3D->CheckDeviceFormat( pCaps->AdapterOrdinal, pCaps->DeviceType,
                    AdapterFormat, D3DUSAGE_QUERY_FILTER | D3DUSAGE_QUERY_WRAPANDMIP, 
                    D3DRTYPE_CUBETEXTURE, D3DFMT_A16B16G16R16F) != D3D_OK )
    {
        return false;
    }

	// Support for D3DFMT_G16R16F cubemaps
    if( FAILED( pD3D->CheckDeviceFormat( pCaps->AdapterOrdinal, pCaps->DeviceType,
                    AdapterFormat, D3DUSAGE_QUERY_FILTER | D3DUSAGE_QUERY_WRAPANDMIP, 
                    D3DRTYPE_CUBETEXTURE, D3DFMT_G16R16F) ) )
    {
        return false;
    }

	// Support for FP16 filtering in D3DFMT_A16B16G16R16F
	if(FAILED(pD3D->CheckDeviceFormat( pCaps->AdapterOrdinal, pCaps->DeviceType,
                                             AdapterFormat, D3DUSAGE_QUERY_FILTER | D3DUSAGE_QUERY_WRAPANDMIP,
                                             D3DRTYPE_TEXTURE, D3DFMT_A16B16G16R16F) ))
	{
		return false;
	}

	// Support for FP16 filtering in D3DFMT_G16R16F
	if(FAILED(pD3D->CheckDeviceFormat( pCaps->AdapterOrdinal, pCaps->DeviceType,
                                             AdapterFormat, D3DUSAGE_QUERY_FILTER | D3DUSAGE_QUERY_WRAPANDMIP,
                                             D3DRTYPE_TEXTURE, D3DFMT_G16R16F) ))
	{
		return false;
	}

	// Support for NP2
	if(!(pCaps->TextureCaps & D3DPTEXTURECAPS_POW2)==0)
	{
		return false;
	}

	// Support for MRT for D3DFMT_G16R16F render targets
	if(!((pCaps->NumSimultaneousRTs>=2) && SUCCEEDED( pD3D->CheckDeviceFormat( pCaps->AdapterOrdinal, pCaps->DeviceType,
                                             AdapterFormat, D3DUSAGE_RENDERTARGET,
                                             D3DRTYPE_TEXTURE, D3DFMT_G16R16F) )))
	{
		return false;
	}


    return true;
}

//--------------------------------------------------------------------------------------
//
//
//--------------------------------------------------------------------------------------
HRESULT HDREngine::onCreateDevice( IDirect3DDevice9* pd3dDevice )
{
    HRESULT hr;
	
	m_pD3DDevice = pd3dDevice;

    // Create the HDR effect
	hr = D3DXCreateEffectFromFile(m_pD3DDevice, GetFilePath::GetFilePath(TEXT("programs\\HLSL_HDR_FP16x2\\HDR.fx"), true).c_str(),
										 NULL, NULL, D3DXSHADER_DEBUG, NULL, &m_pHDREffect, NULL);

	return hr;
}

//--------------------------------------------------------------------------------------
//
//
//--------------------------------------------------------------------------------------

void HDREngine::registerAssets(HDRTexture* envMap, HDRTexture* diffMap, Skybox* skybox, Model3D* model){
	m_pHDREnvMap = envMap;
	m_pHDRDifMap = diffMap;
	m_pSkybox    = skybox;
	m_pModel3D   = model;
}



//--------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------
HRESULT HDREngine::onResetDevice( IDirect3DDevice9* pd3dDevice,  const D3DSURFACE_DESC* pBackBufferSurfaceDesc, bool dualTexture, bool anisoDecimation)
{
    HRESULT hr;

	m_pD3DDevice = pd3dDevice;

	m_pD3DDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &m_pBackBuffer);

    // Recalculate buffer sizes
	m_iScreenWidth   = pBackBufferSurfaceDesc->Width;
	m_iScreenHeight  = pBackBufferSurfaceDesc->Height;

	// Crop the scene texture so width and height are evenly divisible by 8.
    // This cropped version of the scene will be used for post processing effects,
    // and keeping everything evenly divisible allows precise control over
    // sampling points within the shaders.

	m_iCroppedWidth  = pBackBufferSurfaceDesc->Width  - pBackBufferSurfaceDesc->Width  % 8;
	m_iCroppedHeight = pBackBufferSurfaceDesc->Height - pBackBufferSurfaceDesc->Height % 8;
	m_iShrinkWidth   = m_iCroppedWidth / 4;
	m_iShrinkHeight  = m_iCroppedHeight / 4;

	m_bUse2xFp            = dualTexture;
	m_bUseAnisoDecimation = anisoDecimation;

	CalculateOffsetsAndQuadCoords();

	if(m_bUse2xFp){
		D3DXCreateTexture(m_pD3DDevice, m_iScreenWidth, m_iScreenHeight, 1, D3DUSAGE_RENDERTARGET, D3DFMT_G16R16F, D3DPOOL_DEFAULT, &m_pSceneA);
		m_pSceneA->GetSurfaceLevel(0, &m_pSceneASurf);
		D3DXCreateTexture(m_pD3DDevice, m_iScreenWidth, m_iScreenHeight, 1, D3DUSAGE_RENDERTARGET, D3DFMT_G16R16F, D3DPOOL_DEFAULT, &m_pSceneB);
		m_pSceneB->GetSurfaceLevel(0, &m_pSceneBSurf);

		if(m_bUseAnisoDecimation){
			D3DXCreateTexture(m_pD3DDevice, m_iShrinkWidth, m_iScreenHeight/2, 1, D3DUSAGE_RENDERTARGET, D3DFMT_G16R16F, D3DPOOL_DEFAULT, &m_pfp16ShrinkA_1);
			m_pfp16ShrinkA_1->GetSurfaceLevel(0, &m_pfp16ShrinkASurf_1);
		}

		D3DXCreateTexture(m_pD3DDevice, m_iShrinkWidth, m_iShrinkHeight, 1, D3DUSAGE_RENDERTARGET, D3DFMT_G16R16F, D3DPOOL_DEFAULT, &m_pfp16ShrinkA);
		m_pfp16ShrinkA->GetSurfaceLevel(0, &m_pfp16ShrinkASurf);

		D3DXCreateTexture(m_pD3DDevice, m_iShrinkWidth, m_iShrinkHeight, 1, D3DUSAGE_RENDERTARGET, D3DFMT_G16R16F, D3DPOOL_DEFAULT, &m_pfp16ShrinkB);
		m_pfp16ShrinkB->GetSurfaceLevel(0, &m_pfp16ShrinkBSurf);
	
		D3DXCreateTexture(m_pD3DDevice, m_iShrinkWidth, m_iShrinkHeight, 1, D3DUSAGE_RENDERTARGET, D3DFMT_G16R16F, D3DPOOL_DEFAULT, &m_pfp16ShrinkA_2);
		m_pfp16ShrinkA_2->GetSurfaceLevel(0, &m_pfp16ShrinkASurf_2);
	}
	else{
		D3DXCreateTexture(m_pD3DDevice, m_iScreenWidth, m_iScreenHeight, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A16B16G16R16F, D3DPOOL_DEFAULT, &m_pSceneF);
		m_pSceneF->GetSurfaceLevel(0, &m_pSceneFSurf);

		if(m_bUseAnisoDecimation){
			D3DXCreateTexture(m_pD3DDevice, m_iShrinkWidth, m_iScreenHeight/2, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A16B16G16R16F, D3DPOOL_DEFAULT, &m_pfp16ShrinkF_1);
			m_pfp16ShrinkF_1->GetSurfaceLevel(0, &m_pfp16ShrinkFSurf_1);
		}

		// Shrinked RT for blur effect
		D3DXCreateTexture(m_pD3DDevice, m_iShrinkWidth, m_iShrinkHeight, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A16B16G16R16F, D3DPOOL_DEFAULT, &m_pfp16ShrinkF);
		m_pfp16ShrinkF->GetSurfaceLevel(0, &m_pfp16ShrinkFSurf);

		D3DXCreateTexture(m_pD3DDevice, m_iShrinkWidth, m_iShrinkHeight, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A16B16G16R16F, D3DPOOL_DEFAULT, &m_pfp16ShrinkF_2);
		m_pfp16ShrinkF_2->GetSurfaceLevel(0, &m_pfp16ShrinkFSurf_2);
	}

	int dim = 64;
	for(int i = 0; i < MAX_LUM_TEX; i++){
		D3DXCreateTexture(m_pD3DDevice, dim, dim, 1, D3DUSAGE_RENDERTARGET, D3DFMT_G16R16F, D3DPOOL_DEFAULT, &m_pfp16Lum[i]);
		m_pfp16Lum[i]->GetSurfaceLevel(0, &m_pfp16LumSurf[i]);
		dim >>= 2;
	}

	D3DXCreateTexture(m_pD3DDevice, 1, 1, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F, D3DPOOL_DEFAULT, &m_pfp32Lum[0]);
	m_pfp32Lum[0]->GetSurfaceLevel(0, &m_pfp32LumSurf[0]);

	D3DXCreateTexture(m_pD3DDevice, 1, 1, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F, D3DPOOL_DEFAULT, &m_pfp32Lum[1]);
	m_pfp32Lum[1]->GetSurfaceLevel(0, &m_pfp32LumSurf[1]);

	D3DXCreateTexture(m_pD3DDevice, 1, 1, 1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F, D3DPOOL_DEFAULT, &m_pfp32Lum[2]);
	m_pfp32Lum[2]->GetSurfaceLevel(0, &m_pfp32LumSurf[2]);

	m_iPreviousLuminance = 1;

	// Fill the ping-pong luminance textures with some intial value which will be our targeted luminance
	RECT texelRect;

	texelRect.top    = 0;
	texelRect.bottom = 1;
	texelRect.left   = 0;
	texelRect.right  = 1;

	float texel = m_fExposure;
	
	D3DXLoadSurfaceFromMemory(m_pfp32LumSurf[0], NULL, NULL, &texel, D3DFMT_R32F, sizeof(float), NULL, &texelRect, D3DX_FILTER_POINT, 0);
	D3DXLoadSurfaceFromMemory(m_pfp32LumSurf[1], NULL, NULL, &texel, D3DFMT_R32F, sizeof(float), NULL, &texelRect, D3DX_FILTER_POINT, 0);

	return S_OK;
}

//--------------------------------------------------------------------------------------
// This callback function will be called immediately after the Direct3D device has 
// entered a lost state and before IDirect3DDevice9::Reset is called. Resources created
// in the OnResetDevice callback should be released here, which generally includes all 
// D3DPOOL_DEFAULT resources. See the "Lost Devices" section of the documentation for 
// information about lost devices.
//--------------------------------------------------------------------------------------
HRESULT HDREngine::onLostDevice( )
{
 	if( m_pHDREffect )
		m_pHDREffect->OnLostDevice();
	
	if(m_bUse2xFp){
		SAFE_RELEASE(m_pSceneA);
		SAFE_RELEASE(m_pSceneB);
		SAFE_RELEASE(m_pfp16ShrinkA);
		SAFE_RELEASE(m_pfp16ShrinkB);	
		SAFE_RELEASE(m_pfp16ShrinkA_1);
		SAFE_RELEASE(m_pfp16ShrinkA_2);

		SAFE_RELEASE(m_pSceneASurf);
		SAFE_RELEASE(m_pSceneBSurf);
		SAFE_RELEASE(m_pfp16ShrinkASurf);
		SAFE_RELEASE(m_pfp16ShrinkBSurf);
		SAFE_RELEASE(m_pfp16ShrinkASurf_1);
		SAFE_RELEASE(m_pfp16ShrinkASurf_2);
	}
	else{
		SAFE_RELEASE(m_pSceneF);
		SAFE_RELEASE(m_pfp16ShrinkF);
		SAFE_RELEASE(m_pfp16ShrinkF_1);
		SAFE_RELEASE(m_pfp16ShrinkF_2);

		SAFE_RELEASE(m_pSceneFSurf);
		SAFE_RELEASE(m_pfp16ShrinkFSurf);
		SAFE_RELEASE(m_pfp16ShrinkFSurf_1);
		SAFE_RELEASE(m_pfp16ShrinkFSurf_2);
	}

	SAFE_RELEASE(m_pfp32LumSurf[0]);      
	SAFE_RELEASE(m_pfp32LumSurf[1]);   
	SAFE_RELEASE(m_pfp32LumSurf[2]);      
	SAFE_RELEASE(m_pfp32Lum[0]);     
	SAFE_RELEASE(m_pfp32Lum[1]);      
	SAFE_RELEASE(m_pfp32Lum[2]);      

	for(int i = 0; i < MAX_LUM_TEX; i++){
		SAFE_RELEASE(m_pfp16LumSurf[i]);
		SAFE_RELEASE(m_pfp16Lum[i]);
	}


	SAFE_RELEASE(m_pBackBuffer);

	return S_OK;
}


//--------------------------------------------------------------------------------------
//
//
//--------------------------------------------------------------------------------------
HRESULT HDREngine::onDestroyDevice( )
{
 	SAFE_RELEASE(m_pHDREffect);
	return S_OK;
}

//--------------------------------------------------------------------------------------
//
//
//--------------------------------------------------------------------------------------
HRESULT HDREngine::onFrameRender( const D3DXMATRIX *viewMatrix, const D3DXMATRIX *projMatrix, const D3DXMATRIX *worldMatrix, const D3DXVECTOR3 *eyePosition, const float frameDelta)
{
    HRESULT hr;

	// Begin Scene should have been called already
	m_pViewMatrix  = viewMatrix;
	m_pProjMatrix  = projMatrix;
	m_pWorldMatrix = worldMatrix;
	m_pEyePosition = eyePosition;
	
	//Clear targets and z
	m_pD3DDevice->Clear( 0L, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0L );

	if(m_bUse2xFp)
	{
		RenderGeometry(m_bUse2xFp, m_pSceneASurf, m_pSceneBSurf);

		// Calculate luminance
		calculateLuminance(frameDelta, m_pSceneA, m_pSceneB);

		// Glow pass
		glowPass(m_pSceneA, m_pSceneB);
		// Tone Mapping
		ToneMapping(m_pSceneA, m_pSceneB);
	}
	else
	{						
		RenderGeometry(m_bUse2xFp, m_pSceneFSurf);
	
		// Calculate luminance
		calculateLuminance(frameDelta, m_pSceneF);

		// Glow pass
		glowPass(m_pSceneF);

		//Final Blend
		ToneMapping(m_pSceneF);
	}
	m_pD3DDevice->SetRenderTarget(0, m_pBackBuffer);

	return S_OK;
}

//--------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------
void HDREngine::RenderGeometry(bool useMRT, LPDIRECT3DSURFACE9 RenderTarget0, LPDIRECT3DSURFACE9 RenderTarget1)
{
    // Calculate various matrices
	D3DXMATRIX WVPMatrix, VPMatrix;

	D3DXMatrixMultiply( &VPMatrix,  m_pViewMatrix,  m_pProjMatrix);
	D3DXMatrixMultiply( &WVPMatrix, m_pWorldMatrix, m_pViewMatrix );
    D3DXMatrixMultiply( &WVPMatrix, &WVPMatrix,     m_pProjMatrix );
	
	//Render Skybox
	m_pSkybox->Render( &VPMatrix, RenderTarget0, RenderTarget1);

	//Render Model. This pass uses MRT, but could be unfolded so hardware not supporting MRT could do a two pass technique
	m_pD3DDevice->SetRenderTarget(0, RenderTarget0);
	m_pD3DDevice->SetRenderTarget(1, RenderTarget1);
	
    m_pModel3D->Render( m_pWorldMatrix, &WVPMatrix, m_pEyePosition, useMRT);
	
	m_pD3DDevice->SetRenderTarget(1, NULL);
}

//-----------------------------------------------------------------------------
// Name: calculateLuminance
//-----------------------------------------------------------------------------
void HDREngine::calculateLuminance(const float frameDelta, LPDIRECT3DTEXTURE9 channelA, LPDIRECT3DTEXTURE9 channelB)
{
	int i;

	if(m_bUse2xFp)
	{
		// Downscale, blur horizontally and then vertically the first RT and then the second one
		if(m_bUseAnisoDecimation){
			DownscaleSceneAniso(m_pfp16ShrinkASurf, m_pfp16ShrinkASurf_1, channelA, m_pfp16ShrinkA_1);	
			DownscaleSceneAniso(m_pfp16ShrinkBSurf, m_pfp16ShrinkASurf_1, channelB, m_pfp16ShrinkA_1);
		}else{
			DownscaleScene(m_pfp16ShrinkASurf, channelA);	
			DownscaleScene(m_pfp16ShrinkBSurf, channelB);
		}

		LuminancePass(m_pfp16LumSurf[0], LUMINANCE_DOWNSAMPLE_LOG, m_luminanceQuadCoords[0], m_pfp16ShrinkA, m_pfp16ShrinkB);
	}
	else
	{
		if(m_bUseAnisoDecimation){
			DownscaleSceneAniso(m_pfp16ShrinkFSurf, m_pfp16ShrinkFSurf_1, channelA, m_pfp16ShrinkF_1);
		}else{
			DownscaleScene(m_pfp16ShrinkFSurf, channelA);
		}

		LuminancePass(m_pfp16LumSurf[0], LUMINANCE_DOWNSAMPLE_LOG, m_luminanceQuadCoords[0], m_pfp16ShrinkF);
	}

	// Downsample the luminance to a 4x4 texure
	for(int i = 1; i < MAX_LUM_TEX; i++){
		LuminancePass(m_pfp16LumSurf[i], LUMINANCE_DOWNSAMPLE, m_luminanceQuadCoords[i], m_pfp16Lum[i-1]);
	}
	LuminancePass(m_pfp32LumSurf[2], LUMINANCE_DOWNSAMPLE_EXP, m_luminanceQuadCoords[MAX_LUM_TEX], m_pfp16Lum[MAX_LUM_TEX-1]);

	// Temporal adjustment of the luminance
	m_iPreviousLuminance = !m_iPreviousLuminance;
	LuminanceAdaptation(m_pfp32LumSurf[!m_iPreviousLuminance], m_pfp32Lum[m_iPreviousLuminance], m_pfp32Lum[2], frameDelta);
}

//-----------------------------------------------------------------------------
// Name: glowPass
//-----------------------------------------------------------------------------
void HDREngine::glowPass(LPDIRECT3DTEXTURE9 channelA, LPDIRECT3DTEXTURE9 channelB)
{
	// blur horizontally and then vertically the first RT and then the second one
	if(m_bUse2xFp)
	{
		BlurTarget(m_pfp16ShrinkASurf_2, false, m_pfp16ShrinkA);
		BlurTarget(m_pfp16ShrinkASurf  , true,  m_pfp16ShrinkA_2);

		BlurTarget(m_pfp16ShrinkASurf_2, false, m_pfp16ShrinkB );
		BlurTarget(m_pfp16ShrinkBSurf,   true,  m_pfp16ShrinkA_2);
	}
	else
	{
		BlurTarget(m_pfp16ShrinkFSurf_2, false, m_pfp16ShrinkF);
		BlurTarget(m_pfp16ShrinkFSurf,   true,  m_pfp16ShrinkF_2 );
	}
}

//-----------------------------------------------------------------------------
// Name: ToneMapping
//-----------------------------------------------------------------------------
void HDREngine::ToneMapping(LPDIRECT3DTEXTURE9 channelA, LPDIRECT3DTEXTURE9 channelB){
	
	// Set Exposure
	m_pHDREffect->SetValue  ("Exposure", &m_fExposure, sizeof(FLOAT));
	m_pHDREffect->SetTexture("TextureC", m_pfp32Lum[!m_iPreviousLuminance]);
	
	if(m_bUse2xFp)
	{
		m_pHDREffect->SetTexture  ("TextureA", channelA);
		m_pHDREffect->SetTexture  ("TextureB", channelB);	
		m_pHDREffect->SetTexture  ("ScaledTexA", m_pfp16ShrinkA);
		m_pHDREffect->SetTexture  ("ScaledTexB", m_pfp16ShrinkB);
		m_pHDREffect->SetTechnique("ToneMappingx2");
	}
	else
	{	
		m_pHDREffect->SetTexture("TextureA", channelA);
		m_pHDREffect->SetTexture("ScaledTexA", m_pfp16ShrinkF);
		m_pHDREffect->SetTechnique("ToneMapping");
	}

	// Use sRGB to produce the proper gamma correction
	m_pD3DDevice->SetRenderState (D3DRS_SRGBWRITEENABLE, TRUE);
	m_pD3DDevice->SetRenderTarget(0, m_pBackBuffer);

	UINT iPass, iPasses;
	m_pHDREffect->Begin(&iPasses, 0);
	for(iPass = 0; iPass < iPasses; iPass++)
	{
		m_pHDREffect->BeginPass(iPass);
		DrawFullScreenQuad(m_fullscreenVTFTonemapPass, MyFVF0, sizeof(ScreenVertex0)); // Full screen
		m_pHDREffect->EndPass();
	}
	m_pHDREffect->End();

	m_pD3DDevice->SetRenderState(D3DRS_SRGBWRITEENABLE, FALSE);
}



//-----------------------------------------------------------------------------
// Name: DownscaleScene()
//-----------------------------------------------------------------------------
void HDREngine::DownscaleScene(LPDIRECT3DSURFACE9 RT0, LPDIRECT3DTEXTURE9 channelA)
{	
	m_pHDREffect->SetTechnique("Downscale4x4Bilinear");
	m_pHDREffect->SetTexture  ("TextureA", channelA);
	UINT uiPassCount, uiPass;       
        
	m_pD3DDevice->SetRenderTarget( 0, RT0  );
	 
    m_pHDREffect->Begin(&uiPassCount, 0);
    for (uiPass = 0; uiPass < uiPassCount; uiPass++)
    {
        m_pHDREffect->BeginPass(uiPass);
        DrawFullScreenQuad(m_downsampleQuadCoords, MyFVF4, sizeof(ScreenVertex4) );
        m_pHDREffect->EndPass();
    }
    m_pHDREffect->End();
    return;
}

//-----------------------------------------------------------------------------
// Name: DownscaleScene()
//-----------------------------------------------------------------------------
void HDREngine::DownscaleSceneAniso(LPDIRECT3DSURFACE9 RT0, LPDIRECT3DSURFACE9 RT_int, LPDIRECT3DTEXTURE9 channelA, LPDIRECT3DTEXTURE9 Tex_int)
{	
	// First pass (w,h) -> (w/4, h/2) 
	m_pHDREffect->SetTechnique("DownscaleAniso");
	m_pHDREffect->SetTexture  ("TextureA", channelA);
	UINT uiPassCount, uiPass;       
        
	m_pD3DDevice->SetRenderTarget( 0, RT_int  );
	 
    m_pHDREffect->Begin(&uiPassCount, 0);
    for (uiPass = 0; uiPass < uiPassCount; uiPass++)
    {
        m_pHDREffect->BeginPass(uiPass);
        DrawFullScreenQuad(m_downsampleAnisoQuadCoords[0], MyFVF1, sizeof(ScreenVertex1) );
        m_pHDREffect->EndPass();
    }
    m_pHDREffect->End();

	// Second pass (w/4,h/2) -> (w/4, h/4)
	m_pHDREffect->SetTechnique("DownscaleAniso");
	m_pHDREffect->SetTexture  ("TextureA", Tex_int);
       
	m_pD3DDevice->SetRenderTarget( 0, RT0  );
	 
    m_pHDREffect->Begin(&uiPassCount, 0);
    for (uiPass = 0; uiPass < uiPassCount; uiPass++)
    {
        m_pHDREffect->BeginPass(uiPass);
        DrawFullScreenQuad(m_downsampleAnisoQuadCoords[1], MyFVF1, sizeof(ScreenVertex1) );
        m_pHDREffect->EndPass();
    }
    m_pHDREffect->End();

    return;
}

//-----------------------------------------------------------------------------
// Name: DownscaleScene()
//-----------------------------------------------------------------------------
void HDREngine::LuminancePass(LPDIRECT3DSURFACE9 RT0, int method, ScreenVertex4 *vertArray, LPDIRECT3DTEXTURE9 channelA, LPDIRECT3DTEXTURE9 channelB)
{	

	switch(method){
	case LUMINANCE_DOWNSAMPLE_LOG:
		if(channelB)
			m_pHDREffect->SetTechnique("DownscaleLuminanceLogx2");
		else
			m_pHDREffect->SetTechnique("DownscaleLuminanceLog");
	break;
	case LUMINANCE_DOWNSAMPLE:
		m_pHDREffect->SetTechnique("DownscaleLuminance");
	break;     
	case LUMINANCE_DOWNSAMPLE_EXP:
		m_pHDREffect->SetTechnique("DownscaleLuminanceExp");
	break; 
	}
	
	// Set source textures
	m_pHDREffect->SetTexture  ("TextureA", channelA);
	
	if(channelB)
		m_pHDREffect->SetTexture  ("TextureB", channelB);
	
	UINT uiPassCount, uiPass;       
        
	m_pD3DDevice->SetRenderTarget( 0, RT0  );
	 
    m_pHDREffect->Begin(&uiPassCount, 0);
    for (uiPass = 0; uiPass < uiPassCount; uiPass++)
    {
        m_pHDREffect->BeginPass(uiPass);
		DrawFullScreenQuad(vertArray, MyFVF4, sizeof(ScreenVertex4));
        m_pHDREffect->EndPass();
    }
    m_pHDREffect->End();
    return;

}

//-----------------------------------------------------------------------------
// Name: DownscaleScene()
//-----------------------------------------------------------------------------
void HDREngine::LuminanceAdaptation(LPDIRECT3DSURFACE9 dst, LPDIRECT3DTEXTURE9 src1, LPDIRECT3DTEXTURE9 src2, const float frameDelta)
{	
	UINT uiPassCount, uiPass;       
    
	m_pHDREffect->SetTechnique("LuminanceAdaptation");
	
	m_pHDREffect->SetValue ( "FrameDelta", &frameDelta, sizeof(FLOAT));

	// Set source/dest textures
	m_pHDREffect->SetTexture  ("TextureA", src1);
	m_pHDREffect->SetTexture  ("TextureB", src2);
	    
	m_pD3DDevice->SetRenderTarget( 0, dst);
	 
    m_pHDREffect->Begin(&uiPassCount, 0);
    for (uiPass = 0; uiPass < uiPassCount; uiPass++)
    {
        m_pHDREffect->BeginPass(uiPass);
		DrawFullScreenQuad(m_fullscreenQuadCoords, MyFVF1, sizeof(ScreenVertex1));
        m_pHDREffect->EndPass();
    }
    m_pHDREffect->End();

    return;

}
//-----------------------------------------------------------------------------
// Name: BlurTarget()
//-----------------------------------------------------------------------------
void HDREngine::BlurTarget(LPDIRECT3DSURFACE9 RT0, bool verticalDir,  LPDIRECT3DTEXTURE9 channelA)
											
{	
	ScreenVertex8 *coords;

  	if(verticalDir){
		coords = m_blurQuadCoordsV;
	}
	else{
		coords = m_blurQuadCoordsH;
	}

	m_pHDREffect->SetTechnique("Blur1DBilinear");
	m_pHDREffect->SetValue    ("weights", m_gaussWeights, sizeof(m_gaussWeights));
	m_pHDREffect->SetTexture  ("TextureA", channelA);
    
	m_pD3DDevice->SetRenderTarget( 0, RT0 );
	           
    UINT uiPassCount, uiPass;       
    m_pHDREffect->Begin(&uiPassCount, 0);
      
    for (uiPass = 0; uiPass < uiPassCount; uiPass++)
    {
        m_pHDREffect->BeginPass(uiPass);
		DrawFullScreenQuad(coords, MyFVF8, sizeof(ScreenVertex8));
		m_pHDREffect->EndPass();
    }

    m_pHDREffect->End();
}


//-----------------------------------------------------------------------------
// Name: DrawFullScreenQuad
// Desc: Instead of drawing a full screen quad, we use an oversized triangle
// that contains the screen. This will avoid the extra sampling of the edges 
// of the two triangles that the quad generates, saving some cost, especially
// at large resolutions.
//-----------------------------------------------------------------------------
void HDREngine::DrawFullScreenQuad(void *scoords, DWORD theFVF, unsigned int size)
{
   	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
    m_pD3DDevice->SetFVF(theFVF);
    m_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 1, scoords, size);
    m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
}

//-----------------------------------------------------------------------------
// Name: CalculateOffsetsAndQuadCoords
// Desc: Everytime we resize the screen we will recalculate an array of
// vertices and texture coordinates to be use to render the full screen 
// primitive for the different postprocessing effects. Note that different 
// effects use a different set of values! 
//-----------------------------------------------------------------------------
void HDREngine::CalculateOffsetsAndQuadCoords(){
	D3DXVECTOR2 sampleOffsets_scale[MAX_SAMPLES];
	D3DXVECTOR2 sampleOffsets_blurV[MAX_SAMPLES];
	D3DXVECTOR2 sampleOffsets_blurH[MAX_SAMPLES];

	float coordOffsets[MAX_SAMPLES];
	float dU;
    float dV;
	CoordRect tempCoords;

	// Full screen quad coords
	tempCoords.fLeftU   = 0.0f;
	tempCoords.fTopV    = 0.0f;
	tempCoords.fRightU  = 1.0f;
	tempCoords.fBottomV = 1.0f;

	ApplyTexelToPixelCorrection(m_fullscreenVTFTonemapPass, m_iScreenWidth, m_iScreenHeight, &tempCoords);

	ApplyTexelToPixelCorrection(m_fullscreenQuadCoords, m_iScreenWidth, m_iScreenHeight, &tempCoords);

	ApplyTexelToPixelCorrection(m_downsampleAnisoQuadCoords[0], m_iShrinkWidth, m_iScreenHeight/2, &tempCoords);
	ApplyTexelToPixelCorrection(m_downsampleAnisoQuadCoords[1], m_iShrinkWidth, m_iShrinkHeight, &tempCoords);
		
	// Luminance quad coords
	// (w/4, h/4), (64, 64), (16, 16) and (4, 4)
	CalculateSampleOffset_4x4Bilinear( m_iShrinkWidth, m_iShrinkHeight, sampleOffsets_scale );
	ApplyTexelToPixelCorrection(m_luminanceQuadCoords[0], 64, 64, &tempCoords, sampleOffsets_scale);

	CalculateSampleOffset_4x4Bilinear( 64, 64, sampleOffsets_scale );
	ApplyTexelToPixelCorrection(m_luminanceQuadCoords[1], 16, 16, &tempCoords, sampleOffsets_scale);

	CalculateSampleOffset_4x4Bilinear( 16, 16, sampleOffsets_scale );
	ApplyTexelToPixelCorrection(m_luminanceQuadCoords[2], 4, 4, &tempCoords, sampleOffsets_scale);

	CalculateSampleOffset_4x4Bilinear( 4, 4, sampleOffsets_scale );
	ApplyTexelToPixelCorrection(m_luminanceQuadCoords[3], 1, 1, &tempCoords, sampleOffsets_scale);
	

	// Downsampling quad coords
	// Place the rectangle in the center of the back buffer surface
    // Get the texture coordinates for the render target
    dU = 1.0f / m_iScreenWidth;
    dV = 1.0f / m_iScreenHeight;

    tempCoords.fLeftU   = 0.5f * (m_iScreenWidth  - m_iCroppedWidth)  * dU;
    tempCoords.fTopV    = 0.5f * (m_iScreenHeight - m_iCroppedHeight) * dV;
    tempCoords.fRightU  = 1.0f - (m_iScreenWidth  - m_iCroppedWidth)  * dU;
    tempCoords.fBottomV = 1.0f - (m_iScreenHeight - m_iCroppedHeight) * dV;

	CalculateSampleOffset_4x4Bilinear( m_iScreenWidth, m_iScreenHeight, sampleOffsets_scale );
	ApplyTexelToPixelCorrection(m_downsampleQuadCoords, m_iShrinkWidth, m_iShrinkHeight, &tempCoords, sampleOffsets_scale);


	// Blur quad coords.
	tempCoords.fLeftU   = 0.0f;
	tempCoords.fTopV    = 0.0f;
	tempCoords.fRightU  = 1.0f;
	tempCoords.fBottomV = 1.0f;

	CalculateOffsets_GaussianBilinear( (float)m_iShrinkWidth, coordOffsets, m_gaussWeights, 8);
   	for(int i = 0; i < MAX_SAMPLES; i++){
		sampleOffsets_blurH[i].x = coordOffsets[i];
		sampleOffsets_blurH[i].y = 0;
		
		sampleOffsets_blurV[i].x = 0;
		sampleOffsets_blurV[i].y = coordOffsets[i] * m_iShrinkWidth / m_iShrinkHeight;
	}
	ApplyTexelToPixelCorrection(m_blurQuadCoordsV, m_iShrinkWidth, m_iShrinkHeight, &tempCoords, sampleOffsets_blurV);	
	ApplyTexelToPixelCorrection(m_blurQuadCoordsH, m_iShrinkWidth, m_iShrinkHeight, &tempCoords, sampleOffsets_blurH);	
}

//-----------------------------------------------------------------------------
// Name: applyTexelToPixelCorrection
//
// Desc: These set of function will calculate the proper coordinates for the 
// full screen scissored triangle, adding the appropiate offsets (for 
// downsampling and blur) as full coordinates in the texture values. This will
// allow to avoid the calculation of coordinates in the shaders.
//-----------------------------------------------------------------------------
void HDREngine::ApplyTexelToPixelCorrection(ScreenVertex0 dst[], int srcW, int srcH, CoordRect *coords){

    // Ensure that we're directly mapping texels to pixels by offset by 0.5
    // For more info see the doc page titled "Directly Mapping Texels to Pixels"
 	float tWidth   = coords->fRightU  - coords->fLeftU;
    float tHeight  = coords->fBottomV - coords->fTopV;

	float dS = 1.0f/srcW;
	float dT = 1.0f/srcH;

	dst[0].p = D3DXVECTOR4(-1.0f - dS,  1.0f + dT, coords->fLeftU, coords->fTopV);
	dst[1].p = D3DXVECTOR4( 3.0f - dS,  1.0f + dT, coords->fLeftU + (tWidth*2.f), coords->fTopV);
    dst[2].p = D3DXVECTOR4(-1.0f - dS, -3.0f + dT, coords->fLeftU, coords->fTopV + (tHeight*2.f));
}

void HDREngine::ApplyTexelToPixelCorrection(ScreenVertex1 dst[], int srcW, int srcH, CoordRect *coords){

    // Ensure that we're directly mapping texels to pixels by offset by 0.5
    // For more info see the doc page titled "Directly Mapping Texels to Pixels"
    float fWidth5  = (2.f*(float)srcW) - 0.5f;
    float fHeight5 = (2.f*(float)srcH) - 0.5f;
	float tWidth   = coords->fRightU  - coords->fLeftU;
    float tHeight  = coords->fBottomV - coords->fTopV;

    dst[0].p = D3DXVECTOR4(-0.5f, -0.5f, 0.5f, 1.0f);
    dst[0].t = D3DXVECTOR2(coords->fLeftU, coords->fTopV);

    dst[1].p = D3DXVECTOR4(fWidth5, -0.5f, 0.5f, 1.f);
    dst[1].t = D3DXVECTOR2(coords->fLeftU + (tWidth*2.f), coords->fTopV);

    dst[2].p = D3DXVECTOR4(-0.5f, fHeight5, 0.5f, 1.f);
    dst[2].t = D3DXVECTOR2(coords->fLeftU, coords->fTopV + (tHeight*2.f));
}

//-----------------------------------------------------------------------------
void HDREngine::ApplyTexelToPixelCorrection(ScreenVertex4 dst[], int srcW, int srcH, CoordRect *coords, D3DXVECTOR2 offsets[]){

    // Ensure that we're directly mapping texels to pixels by offset by 0.5
    // For more info see the doc page titled "Directly Mapping Texels to Pixels"
    float fWidth5  = (2.f*(float)srcW) - 0.5f;
    float fHeight5 = (2.f*(float)srcH) - 0.5f;
	float tWidth   = coords->fRightU  - coords->fLeftU;
    float tHeight  = coords->fBottomV - coords->fTopV;

	// Position
	dst[0].p  = D3DXVECTOR4(  -0.5f,    -0.5f, 0.5f, 1.0f);
	dst[1].p  = D3DXVECTOR4(fWidth5,    -0.5f, 0.5f, 1.0f);
	dst[2].p  = D3DXVECTOR4(  -0.5f, fHeight5, 0.5f, 1.0f);

	// Offsets
	float tempTexCoords[8];
	float tempX[3], tempY[3];
	int i, j;

	tempX[0] = coords->fLeftU;
	tempY[0] = coords->fTopV;
	
	tempX[1] = coords->fLeftU + (tWidth*2.f);
	tempY[1] = coords->fTopV;

	tempX[2] = coords->fLeftU;
	tempY[2] = coords->fTopV + (tHeight*2.f);
	
	for(i = 0; i < 3; i++){
		for(j = 0; j < 4; j++ ){
			tempTexCoords[2*j+0] = tempX[i] + offsets[j].x;
			tempTexCoords[2*j+1] = tempY[i] + offsets[j].y;
		}
		dst[i].t[0] = D3DXVECTOR4(&tempTexCoords[0]);
		dst[i].t[1] = D3DXVECTOR4(&tempTexCoords[4]);
	}
}


//-----------------------------------------------------------------------------
void HDREngine::ApplyTexelToPixelCorrection(ScreenVertex8 dst[], int srcW, int srcH, CoordRect *coords, D3DXVECTOR2 offsets[]){

    // Ensure that we're directly mapping texels to pixels by offset by 0.5
    // For more info see the doc page titled "Directly Mapping Texels to Pixels"
    float fWidth5  = (2.f*(float)srcW) - 0.5f;
    float fHeight5 = (2.f*(float)srcH) - 0.5f;
	float tWidth   = coords->fRightU - coords->fLeftU;
    float tHeight  = coords->fBottomV - coords->fTopV;

	// Position
	dst[0].p  = D3DXVECTOR4(  -0.5f,    -0.5f, 0.5f, 1.0f);
	dst[1].p  = D3DXVECTOR4(fWidth5,    -0.5f, 0.5f, 1.0f);
	dst[2].p  = D3DXVECTOR4(  -0.5f, fHeight5, 0.5f, 1.0f);

	// Offsets
	float tempTexCoords[16];
	float tempX[3], tempY[3];
	int i, j;

	tempX[0] = coords->fLeftU;
	tempY[0] = coords->fTopV;
	
	tempX[1] = coords->fLeftU + (tWidth*2.f);
	tempY[1] = coords->fTopV;

	tempX[2] = coords->fLeftU;
	tempY[2] = coords->fTopV + (tHeight*2.f);
	
	for(i = 0; i < 3; i++){
		for(j = 0; j < 8; j++ ){
			tempTexCoords[2*j+0] = tempX[i] + offsets[j].x;
			tempTexCoords[2*j+1] = tempY[i] + offsets[j].y;
		}
		dst[i].t[0] = D3DXVECTOR4(&tempTexCoords[0]);
		dst[i].t[1] = D3DXVECTOR4(&tempTexCoords[4]);
		dst[i].t[2] = D3DXVECTOR4(&tempTexCoords[8]);
		dst[i].t[3] = D3DXVECTOR4(&tempTexCoords[12]);
	}
}


//-----------------------------------------------------------------------------
// Name: CalculateSampleOffset_4x4Bilinear
// Desc: 
//-----------------------------------------------------------------------------
void HDREngine::CalculateSampleOffset_4x4Bilinear( DWORD dwWidth, DWORD dwHeight, D3DXVECTOR2 avSampleOffsets[] )
{
    float tU = 1.0f / dwWidth;
    float tV = 1.0f / dwHeight;

    // Sample from the 16 surrounding points.  Since bilinear filtering is being used, specific the coordinate
    // exactly halfway between the current texel center (k-1.5) and the neighboring texel center (k-0.5)

    int index=0;
    for( int y=0; y < 4; y+=2 )
    {
        for( int x=0; x < 4; x+=2, index++ )
        {
            avSampleOffsets[ index ].x = (x - 1.f) * tU;
            avSampleOffsets[ index ].y = (y - 1.f) * tV;
        }
    }
}


//-----------------------------------------------------------------------------
// Calculate Gaussian weights based on kernel size
//-----------------------------------------------------------------------------
// generate array of weights for Gaussian blur
float* generateGaussianWeights(int kernelRadius)
{
    int i;
    int size = kernelRadius*2;
    
	float x;
	float s        = kernelRadius / 3.0f;  
	float *weights = new float [size];

    float sum = 0.0;
    for(i=0; i < size; i++) {
		x          = (float) i - kernelRadius + 0.5;
        weights[i] = (float)(exp(-x*x/(2*s*s)) / (s*sqrt(2*D3DX_PI)));
        sum       += weights[i];
    }

    for(i=0; i < size; i++) {
        weights[i] /= sum;
    }
    return weights;
}

//-----------------------------------------------------------------------------
// Name: CalculateOffsets_GaussianBilinear
//
//  We want the general convolution:
//    a*f(i) + b*f(i+1)
//  Linear texture filtering gives us:
//    f(x) = (1-alpha)*f(i) + alpha*f(i+1);
//  It turns out by using the correct weight and offset we can use a linear lookup to achieve this:
//    (a+b) * f(i + b/(a+b))
//  as long as 0 <= b/(a+b) <= 1.
//
//  Given a standard deviation, we can calculate the size of the kernel and viceversa.
//
//-----------------------------------------------------------------------------
void HDREngine::CalculateOffsets_GaussianBilinear( float texSize, float *coordOffset, float *gaussWeight, int kernelRadius )
{
    int i=0;
    float du = 1.0f / texSize;

    //  store all the intermediate offsets & weights, then compute the bilinear
    //  taps in a second pass
    float *tmpWeightArray = generateGaussianWeights(kernelRadius);
    float *tmpOffsetArray = new float[2*kernelRadius];

	// Fill the offsets
    for( i = 0; i < kernelRadius; i++ )
    {
        tmpOffsetArray[i]                = -(float)(kernelRadius - i) * du;
		tmpOffsetArray[i + kernelRadius] =  (float)(i + 1) * du;
    }

	// Bilinear filtering taps 
	// Ordering is left to right.
    for( i=0; i < kernelRadius; i++ )
    {
        float sScale   = tmpWeightArray[i*2] + tmpWeightArray[i*2+1];
        float sFrac    = tmpWeightArray[i*2] / sScale;

        coordOffset[i] = (tmpOffsetArray[i*2] + (1-sFrac))*du;
        gaussWeight[i] = sScale;
    }
}
