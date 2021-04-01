#pragma once
//--------------------------------------------------------------------------------------
// UI control IDs
//--------------------------------------------------------------------------------------
#include "skybox.h"
#include "model3D.h"
#include "HDRTexture.h"
#include "HDREngine.h"

enum
{
IDC_TOGGLEFULLSCREEN,
IDC_TOGGLEREF,
IDC_CHANGEDEVICE,

IDC_2XFP_LABEL,
IDC_2XFP_RADIOA,
IDC_2XFP_RADIOB,

IDC_ANISO_CHECKBOX,

IDC_REFL_LABEL,
IDC_REFL_RADIOA,
IDC_REFL_RADIOB,

IDC_EXP_LABEL,
IDC_EXPOSURE,

IDC_LAST,
};
//--------------------------------------------------------------------------------------
// Forward declarations 
//--------------------------------------------------------------------------------------
bool    CALLBACK IsDeviceAcceptable  ( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, bool bWindowed, void* pUserContext );
bool    CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, const D3DCAPS9* pCaps, void* pUserContext );
HRESULT CALLBACK OnCreateDevice      ( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
HRESULT CALLBACK OnResetDevice       ( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
void    CALLBACK OnFrameMove         ( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext );
void    CALLBACK OnFrameRender       ( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext );
LRESULT CALLBACK MsgProc             ( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void* pUserContext );
void    CALLBACK KeyboardProc        ( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext );
void    CALLBACK OnGUIEvent          ( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext );
void    CALLBACK OnLostDevice        ( void* pUserContext );
void    CALLBACK OnDestroyDevice     ( void* pUserContext );

void    InitApp();
void    RenderText();

//--------------------------------------------------------------------------------------
// Global Variables
//--------------------------------------------------------------------------------------
CModelViewerCamera         g_Camera;                // A model viewing camera
CDXUTDialog                g_HUD;                   // dialog for standard controls
CDXUTDialogResourceManager g_DialogResourceManager; // manager for shared resources of dialogs
CD3DSettingsDlg            g_SettingsDlg;           // Device settings dialog
ID3DXFont*                 g_pFont       = NULL;    // Font for drawing text
ID3DXSprite*               g_pTextSprite = NULL;    // Sprite for batching draw text calls
bool					   g_bShowHelp   = false;
bool					   g_bShowUI     = true;
bool					   g_bUse2xFp    = true;
bool					   g_bUseAniso   = true;
bool					   g_bReflect    = true;
float                      g_fExposure   = 0.18f;

int                        g_iScreenWidth  = 0;
int                        g_iScreenHeight = 0;

HDREngine*                 g_pHDREngine;
HDRTexture*                g_pHDREnvMap;
HDRTexture*                g_pHDRDifMap;
Skybox*                    g_pSkybox;
Model3D*                   g_pModel3D;
