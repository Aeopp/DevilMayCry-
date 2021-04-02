#define STRICT
#include "nvafx.h"
#include "HDR_FP16x2.h"
#include <math.h>
#include <shared/GetFilePath.h>
#include <d3d9.h>
#include <d3dx9.h>

#define INITIAL_WIDTH        640    
#define INITIAL_HEIGHT       480

#define ENVMAP_GRACEX          0
#define ENVMAP_FOREST          1
#define ENVMAP_SPETER          2

#define MODEL_VENUS            0
#define MODEL_TEAPT            1
#define MODEL_HAPPY            2
#define MODEL_BUNNY            3

int useEnvMap = ENVMAP_GRACEX;
int useModel  = MODEL_VENUS;

// My own fullscreen toggle function to avoind vsync when going to fullscreen
HRESULT HDRFP16x2_ToggleFullScreen();

#define NUMBER_OF_MIDGREYLEVELS 11
#define INTIAL_GREYLEVEL         5
float middleGreyKey[] = { 0.0, 0.01125, 0.0225, 0.045, 0.09, 0.18, 0.36, 0.72, 1.44, 2.88, 5.76} ; 

//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing 
// loop. Idle time is used to render the scene.
//--------------------------------------------------------------------------------------
INT WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
    // Set the callback functions. These functions allow the sample framework to notify
    // the application about device changes, user input, and windows messages.  The 
    // callbacks are optional so you need only set callbacks for events you're interested 
    // in. However, if you don't handle the device reset/lost callbacks then the sample 
    // framework won't be able to reset your device since the application must first 
    // release all device resources before resetting.  Likewise, if you don't handle the 
    // device created/destroyed callbacks then the sample framework won't be able to 
    // recreate your device resources.
    DXUTSetCallbackDeviceCreated  ( OnCreateDevice );
    DXUTSetCallbackDeviceReset    ( OnResetDevice );
    DXUTSetCallbackDeviceLost     ( OnLostDevice );
    DXUTSetCallbackDeviceDestroyed( OnDestroyDevice );
    DXUTSetCallbackMsgProc        ( MsgProc );
    DXUTSetCallbackKeyboard       ( KeyboardProc );
    DXUTSetCallbackFrameRender    ( OnFrameRender );
    DXUTSetCallbackFrameMove      ( OnFrameMove );
	
    // Show the cursor and clip it when in full screen
    DXUTSetCursorSettings( true, true );

    InitApp();

    // Initialize the sample framework and create the desired Win32 window and Direct3D 
    // device for the application. Calling each of these functions is optional, but they
    // allow you to set several options which control the behavior of the framework.

    DXUTInit( true, true, true ); // Parse the command line, handle the default hotkeys, and show msgboxes
    DXUTCreateWindow( L"HDR FP16x2" );
    DXUTCreateDevice( D3DADAPTER_DEFAULT, true, INITIAL_WIDTH, INITIAL_HEIGHT, IsDeviceAcceptable, ModifyDeviceSettings );

    // Pass control to the sample framework for handling the message pump and 
    // dispatching render calls. The sample framework will call your FrameMove 
    // and FrameRender callback when there is idle time between handling window messages.
    DXUTMainLoop();

    // Perform any application-level cleanup here. Direct3D device resources are released within the
    // appropriate callback functions and therefore don't require any cleanup code here.
    return DXUTGetExitCode();
}


//--------------------------------------------------------------------------------------
// Initialize the app 
//--------------------------------------------------------------------------------------
void InitApp()
{
    // Initialize controls
    g_SettingsDlg.Init( &g_DialogResourceManager );
    g_HUD.Init( &g_DialogResourceManager );
    g_HUD.SetCallback( OnGUIEvent );
    g_HUD.AddButton     ( IDC_TOGGLEFULLSCREEN, L"Toggle full screen",      35, 34, 125, 22 );
    g_HUD.AddButton     ( IDC_TOGGLEREF,        L"Toggle REF (F3)",         35, 58, 125, 22, VK_F3 );
    g_HUD.AddButton     ( IDC_CHANGEDEVICE,     L"Change device (F2)",      35, 82, 125, 22, VK_F2 );

	g_HUD.AddStatic     ( IDC_2XFP_LABEL,       L"Toggle 2xRT/1xRT",        35, 324, 125, 22 );
	g_HUD.AddRadioButton( IDC_2XFP_RADIOA, 1,   L"2 RT (Fast)",              0, 50, 200, 24, g_bUse2xFp  );
    g_HUD.AddRadioButton( IDC_2XFP_RADIOB, 1,   L"1 RT (Slow)",              0, 50, 200, 24, !g_bUse2xFp  );

	g_HUD.AddCheckBox   ( IDC_ANISO_CHECKBOX,   L"Use aniso decimation",     0, 50, 200, 24, g_bUseAniso );
    
	g_HUD.AddStatic     ( IDC_REFL_LABEL,       L"Solid/Refractive object", 35, 324, 125, 22 );
	g_HUD.AddRadioButton( IDC_REFL_RADIOA, 2,   L"Solid",                    0, 50, 200, 24 );
    g_HUD.AddRadioButton( IDC_REFL_RADIOB, 2,   L"Refractive",               0, 50, 200, 24 );

	g_HUD.AddStatic     ( IDC_EXP_LABEL,        L"Exposure",                35, 324, 125, 22 );
    g_HUD.AddSlider     ( IDC_EXPOSURE,                                     35, 348, 125, 22, 0, NUMBER_OF_MIDGREYLEVELS - 1, INTIAL_GREYLEVEL );
	
	g_HUD.GetRadioButton( IDC_2XFP_RADIOA )->SetChecked(true);
	g_HUD.GetRadioButton( IDC_REFL_RADIOA )->SetChecked(true);

	WCHAR label[256] = {0};
	_snwprintf( label, 255, L"Midgrey level: %.4f", middleGreyKey[INTIAL_GREYLEVEL] );
    g_HUD.GetStatic( IDC_EXP_LABEL )->SetText( label );

	// Initialize supporting objects.
    g_pHDREngine = new HDREngine;
	g_pHDREnvMap = new HDRTexture;
	g_pHDRDifMap = new HDRTexture;
	g_pSkybox    = new Skybox;
	g_pModel3D   = new Model3D;

	// Register HDR components on the HDREngine
	g_pHDREngine->registerAssets(g_pHDREnvMap, g_pHDRDifMap, g_pSkybox, g_pModel3D);
}


//--------------------------------------------------------------------------------------
// Called during device initialization, this code checks the device for some 
// minimum set of capabilities, and rejects those that don't pass by returning false.
//--------------------------------------------------------------------------------------
bool CALLBACK IsDeviceAcceptable( D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, 
                                  D3DFORMAT BackBufferFormat, bool bWindowed, void* pUserContext )
{
    
	IDirect3D9* pD3D = DXUTGetD3DObject(); 
	
	return g_pHDREngine->isDeviceAcceptable(pD3D, pCaps, AdapterFormat);

}

//--------------------------------------------------------------------------------------
// This callback function is called immediately before a device is created to allow the 
// application to modify the device settings. The supplied pDeviceSettings parameter 
// contains the settings that the framework has selected for the new device, and the 
// application can make any desired changes directly to this structure.  Note however that 
// the sample framework will not correct invalid device settings so care must be taken 
// to return valid device settings, otherwise IDirect3D9::CreateDevice() will fail.  
//--------------------------------------------------------------------------------------
bool CALLBACK ModifyDeviceSettings( DXUTDeviceSettings* pDeviceSettings, const D3DCAPS9* pCaps, void* pUserContext )
{
	pDeviceSettings->pp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	pDeviceSettings->pp.BackBufferCount      = 1;

    return true;
}


//--------------------------------------------------------------------------------------
// This callback function will be called immediately after the Direct3D device has been 
// created, which will happen during application initialization and windowed/full screen 
// toggles. This is the best location to create D3DPOOL_MANAGED resources since these 
// resources need to be reloaded whenever the device is destroyed. Resources created  
// here should be released in the OnDestroyDevice callback. 
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
    HRESULT hr;

    V_RETURN( g_DialogResourceManager.OnCreateDevice( pd3dDevice ) );
    V_RETURN( g_SettingsDlg.OnCreateDevice( pd3dDevice ) );
	
	g_pHDREngine->onCreateDevice(pd3dDevice);

   	// Load the HDR texture
	switch(useEnvMap){
	case ENVMAP_GRACEX: 
		g_pHDREnvMap->LoadTexture(L"textures\\hdr\\grace_cross.hdr");   
		g_pHDRDifMap->LoadTexture(L"textures\\hdr\\grace_cross_diffuse.hdr");
		break;
	case ENVMAP_FOREST: 
		g_pHDREnvMap->LoadTexture(L"textures\\hdr\\rnl_cross.hdr");     
		g_pHDRDifMap->LoadTexture(L"textures\\hdr\\rnl_cross_diffuse.hdr"); 
		break;
	case ENVMAP_SPETER: 
		g_pHDREnvMap->LoadTexture(L"textures\\hdr\\stpeters_cross.hdr"); 
		g_pHDRDifMap->LoadTexture(L"textures\\hdr\\stpeters_cross_diffuse.hdr"); 
		break;
	}

	// Create the Mesh
	switch(useModel){
	case MODEL_VENUS: 
		g_pModel3D->OnCreateDevice(pd3dDevice, L"models\\HLSL_HDR_FP16x2\\venus.x", L"programs\\HLSL_HDR_FP16x2\\Model3D.fx");
		break;
	case MODEL_TEAPT: 
		g_pModel3D->OnCreateDevice(pd3dDevice, L"models\\HLSL_HDR_FP16x2\\teapot.x", L"programs\\HLSL_HDR_FP16x2\\Model3D.fx");
		break;
	case MODEL_HAPPY: 
		g_pModel3D->OnCreateDevice(pd3dDevice, L"models\\HLSL_HDR_FP16x2\\happy.x", L"programs\\HLSL_HDR_FP16x2\\Model3D.fx");
		break;
	case MODEL_BUNNY: 
		g_pModel3D->OnCreateDevice(pd3dDevice, L"models\\HLSL_HDR_FP16x2\\bunny.x", L"programs\\HLSL_HDR_FP16x2\\Model3D.fx");
		break;
	}
		
	g_pModel3D->SetReflectiveMode(g_bReflect);

	// Create the SkyBox
	g_pSkybox->OnCreateDevice( pd3dDevice, L"programs\\HLSL_HDR_FP16x2\\Skybox.fx" );
	
	// Initialize the font
    D3DXCreateFont( pd3dDevice, 15, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET, 
                        OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, 
                        L"Arial", &g_pFont );

    // Set up our view matrix. A view matrix can be defined given an eye point and
    // a point to lookat. Here, we set the eye five units back along the z-axis and 
	// up three units and look at the origin.
    D3DXVECTOR3 vFromPt   = D3DXVECTOR3(0.0f, 0.0f, -4.0f);
    D3DXVECTOR3 vLookatPt = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Camera.SetViewParams( &vFromPt, &vLookatPt);

	

	return S_OK;
}


//--------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnResetDevice( IDirect3DDevice9* pd3dDevice,  const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
    HRESULT hr;

    V_RETURN( g_DialogResourceManager.OnResetDevice() );
    V_RETURN( g_SettingsDlg.OnResetDevice() );

	g_iScreenWidth   = pBackBufferSurfaceDesc->Width;
	g_iScreenHeight  = pBackBufferSurfaceDesc->Height;

	// Delegate on HDREngine all the rendertarget creation
	g_pHDREngine->onResetDevice(pd3dDevice, pBackBufferSurfaceDesc, g_bUse2xFp, g_bUseAniso);

    // Create the cubemaps
	g_pHDREnvMap->CreateCubemaps(pd3dDevice, g_bUse2xFp);
	g_pHDRDifMap->CreateCubemaps(pd3dDevice, g_bUse2xFp);
	
	g_pModel3D->SetEnvironmentMaps(g_pHDREnvMap->m_pCubeMapF, g_pHDREnvMap->m_pCubeMapA, g_pHDREnvMap->m_pCubeMapB);
	g_pModel3D->SetDiffuseMaps    (g_pHDRDifMap->m_pCubeMapF, g_pHDRDifMap->m_pCubeMapA, g_pHDRDifMap->m_pCubeMapB);
	g_pSkybox->SetEnvironmentMaps (g_pHDREnvMap->m_pCubeMapF, g_pHDREnvMap->m_pCubeMapA, g_pHDREnvMap->m_pCubeMapB);

	g_pHDREngine->setExposure( g_fExposure );
	
    // Setup the camera's projection parameters
    float fAspectRatio = g_iScreenWidth / (FLOAT)g_iScreenHeight;
    g_Camera.SetProjParams( D3DX_PI/4, fAspectRatio, 0.1f, 1000.0f );
    g_Camera.SetWindow( g_iScreenWidth, g_iScreenHeight );

    g_HUD.SetLocation( 0, 0 );
    g_HUD.SetSize( g_iScreenWidth, g_iScreenHeight );

	int iY = 15;
    g_HUD.GetControl( IDC_TOGGLEFULLSCREEN )->SetLocation( g_iScreenWidth - 135, iY);
    g_HUD.GetControl( IDC_TOGGLEREF )->SetLocation       ( g_iScreenWidth - 135, iY += 24 );
    g_HUD.GetControl( IDC_CHANGEDEVICE )->SetLocation    ( g_iScreenWidth - 135, iY += 24 );

	g_HUD.GetControl( IDC_2XFP_LABEL )->SetLocation      ( g_iScreenWidth - 135, iY += 48 );
	g_HUD.GetControl( IDC_2XFP_RADIOA )->SetLocation     ( g_iScreenWidth - 135, iY += 24 );
    g_HUD.GetControl( IDC_2XFP_RADIOB )->SetLocation     ( g_iScreenWidth - 135, iY += 24 );
    
	g_HUD.GetControl( IDC_ANISO_CHECKBOX )->SetLocation  ( g_iScreenWidth - 135, iY += 48 );

	g_HUD.GetControl( IDC_REFL_LABEL )->SetLocation      ( g_iScreenWidth - 135, iY += 48 );
	g_HUD.GetControl( IDC_REFL_RADIOA )->SetLocation     ( g_iScreenWidth - 135, iY += 24 );
    g_HUD.GetControl( IDC_REFL_RADIOB )->SetLocation     ( g_iScreenWidth - 135, iY += 24 );

	g_HUD.GetControl( IDC_EXP_LABEL )->SetLocation       ( g_iScreenWidth - 135, iY += 48 );
    g_HUD.GetControl( IDC_EXPOSURE )->SetLocation        ( g_iScreenWidth - 135, iY += 24 );

	
    pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

	if( g_pFont )
        g_pFont->OnResetDevice();
	
	if( g_pSkybox )
		g_pSkybox->OnResetDevice(pBackBufferSurfaceDesc);
	
	if(g_pTextSprite)
		g_pTextSprite->OnResetDevice();
	else
		D3DXCreateSprite( pd3dDevice, &g_pTextSprite );

	return S_OK;
}


//--------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------
void CALLBACK OnFrameMove( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext )
{
	g_Camera.FrameMove( fElapsedTime );
}


//--------------------------------------------------------------------------------------
// Render the help and statistics text. This function uses the ID3DXFont interface for 
// efficient text rendering.
//--------------------------------------------------------------------------------------
void RenderText()
{
    // The helper object simply helps keep track of text position, and color
    // and then it calls pFont->DrawText( m_pSprite, strMsg, -1, &rc, DT_NOCLIP, m_clr );
    // If NULL is passed in as the sprite object, then it will work however the 
    // pFont->DrawText() will not be batched together.  Batching calls will improves performance.
    CDXUTTextHelper txtHelper ( g_pFont,  g_pTextSprite, 15 );
	
    const D3DSURFACE_DESC* pd3dsdBackBuffer = DXUTGetBackBufferSurfaceDesc();
	
	// Output statistics
	txtHelper.Begin();
	txtHelper.SetInsertionPos( 5, 5 );
	txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f ) );
	
	if( g_bShowUI )
	{
		txtHelper.DrawTextLine( DXUTGetFrameStats() );
		txtHelper.DrawTextLine( DXUTGetDeviceStats() );
		
		if( !g_bShowHelp )
		{
			txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ));
			txtHelper.DrawTextLine( TEXT("F1      - Toggle help text") );
		}
	}

	if( g_bShowHelp )
	{
		// Display help text here
		txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
		txtHelper.DrawTextLine( TEXT("F1      - Toggle help text") );
		txtHelper.DrawTextLine( TEXT("H       - Toggle UI") );
		txtHelper.DrawTextLine( TEXT("ESC  - Quit") );
	}
	txtHelper.End();

	if( g_bShowUI )
	{
		// Display any additional information text here
		txtHelper.Begin();
		txtHelper.SetInsertionPos(5, g_iScreenHeight - 25 );
		txtHelper.SetForegroundColor(D3DXCOLOR( 1.0f, 0.0f, 0.0f, 1.0f ));

		if(g_bUse2xFp){
			if(g_bUseAniso){
				txtHelper.DrawTextLine( TEXT("FAST Mode - Using 2 x G16R16F render rargets with aniso decimation.") );
			}else{
				txtHelper.DrawTextLine( TEXT("FAST Mode - Using 2 x G16R16F render rargets.") );
			}
		}else{
			if(g_bUseAniso){
				txtHelper.DrawTextLine( TEXT("SLOW Mode - Using 1 x A16B16G16R16F  render rarget with aniso decimation.") );
			}else{
				txtHelper.DrawTextLine( TEXT("SLOW Mode - Using 1 x A16B16G16R16F  render rarget.") );
			}
		}
		txtHelper.End();
	}
}


//--------------------------------------------------------------------------------------
// Before handling window messages, the sample framework passes incoming windows 
// messages to the application through this callback function. If the application sets 
// *pbNoFurtherProcessing to TRUE, then the sample framework will not process this message.
//--------------------------------------------------------------------------------------
LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void* pUserContext )
{
    // Always allow dialog resource manager calls to handle global messages
    // so GUI state is updated correctly
    g_DialogResourceManager.MsgProc( hWnd, uMsg, wParam, lParam );

    if( g_SettingsDlg.IsActive() )
    {
        g_SettingsDlg.MsgProc( hWnd, uMsg, wParam, lParam );
        return 0;
    }

    // Give the dialogs a chance to handle the message first
    *pbNoFurtherProcessing = g_HUD.MsgProc( hWnd, uMsg, wParam, lParam );
    if( *pbNoFurtherProcessing )
        return 0;

    // Pass all remaining windows messages to camera so it can respond to user input
    g_Camera.HandleMessages( hWnd, uMsg, wParam, lParam );

    return 0;
}


//--------------------------------------------------------------------------------------
// As a convenience, the sample framework inspects the incoming windows messages for
// keystroke messages and decodes the message parameters to pass relevant keyboard
// messages to the application.  The framework does not remove the underlying keystroke 
// messages, which are still passed to the application's MsgProc callback.
//--------------------------------------------------------------------------------------
void CALLBACK KeyboardProc( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext )
{
	if( bKeyDown )
	{
		switch( nChar )
		{
		case VK_F1:
			g_bShowHelp = !g_bShowHelp;
			break;

		case 'H':
		case 'h':
			g_bShowUI = !g_bShowUI;
			for( int i = 0; i < IDC_LAST; i++ )
				g_HUD.GetControl(i)->SetVisible( g_bShowUI );
			break;
		}
	}
}

//--------------------------------------------------------------------------------------
// Handles the GUI events
//--------------------------------------------------------------------------------------
void CALLBACK OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
	WCHAR label[256] = {0};

	switch( nControlID )
    {
        case IDC_TOGGLEFULLSCREEN: 
				HDRFP16x2_ToggleFullScreen();
			break;
        case IDC_TOGGLEREF:        DXUTToggleREF(); break;
        case IDC_CHANGEDEVICE:     g_SettingsDlg.SetActive( !g_SettingsDlg.IsActive() ); break;
		
		case IDC_2XFP_RADIOA:
			OnLostDevice(pUserContext);
			g_bUse2xFp = true;
			OnResetDevice(DXUTGetD3DDevice(), DXUTGetBackBufferSurfaceDesc(), pUserContext);
			break;
		case IDC_2XFP_RADIOB:
			OnLostDevice(pUserContext);
			g_bUse2xFp = false;
			OnResetDevice(DXUTGetD3DDevice(), DXUTGetBackBufferSurfaceDesc(), pUserContext);
            break;
		
		case IDC_ANISO_CHECKBOX:
			{
			OnLostDevice(pUserContext);
			
			CDXUTCheckBox* pCheckBox = (CDXUTCheckBox*) pControl;
			g_bUseAniso = pCheckBox->GetChecked();

			OnResetDevice(DXUTGetD3DDevice(), DXUTGetBackBufferSurfaceDesc(), pUserContext);
            break;
			}

		case IDC_REFL_RADIOA:
			g_bReflect = true; 
			g_pModel3D->SetReflectiveMode(g_bReflect);
			break;

        case IDC_REFL_RADIOB:	       
			g_bReflect = false; 
			g_pModel3D->SetReflectiveMode(g_bReflect);
			break;
		
		case IDC_EXPOSURE:
        {
			CDXUTStatic* pStatic = g_HUD.GetStatic( IDC_EXP_LABEL );
			CDXUTSlider* pSlider = (CDXUTSlider*) pControl;
			
			g_fExposure = middleGreyKey[pSlider->GetValue()];

			_snwprintf( label, 255, L"Midgrey level: %.4f", g_fExposure );
            pStatic->SetText( label );

			g_pHDREngine->setExposure( g_fExposure );
            
            break;
        }
    }
}


//--------------------------------------------------------------------------------------
// This callback function will be called immediately after the Direct3D device has 
// entered a lost state and before IDirect3DDevice9::Reset is called. Resources created
// in the OnResetDevice callback should be released here, which generally includes all 
// D3DPOOL_DEFAULT resources. See the "Lost Devices" section of the documentation for 
// information about lost devices.
//--------------------------------------------------------------------------------------
void CALLBACK OnLostDevice( void* pUserContext )
{
    g_DialogResourceManager.OnLostDevice();
    g_SettingsDlg.OnLostDevice();

    if( g_pFont )
        g_pFont->OnLostDevice();

	
	if( g_pSkybox )
		g_pSkybox->OnLostDevice();
	
	if( g_pModel3D )
		g_pModel3D->OnLostDevice();

	if( g_pTextSprite)
		g_pTextSprite->OnLostDevice();

	if( g_pHDREngine )
		g_pHDREngine->onLostDevice();

}


//--------------------------------------------------------------------------------------
// This callback function will be called immediately after the Direct3D device has 
// been destroyed, which generally happens as a result of application termination or 
// windowed/full screen toggles. Resources created in the OnCreateDevice callback 
// should be released here, which generally includes all D3DPOOL_MANAGED resources. 
//--------------------------------------------------------------------------------------
void CALLBACK OnDestroyDevice( void* pUserContext )
{
    g_DialogResourceManager.OnDestroyDevice();
    g_SettingsDlg.OnDestroyDevice();

    SAFE_RELEASE(g_pFont);
	SAFE_RELEASE(g_pTextSprite);

	g_pSkybox->OnDestroyDevice();
	g_pModel3D->OnDestroyDevice();
	g_pHDREnvMap->OnDestroyDevice();
	g_pHDRDifMap->OnDestroyDevice();
	g_pHDREngine->onDestroyDevice();
}

//--------------------------------------------------------------------------------------
// This callback function will be called at the end of every frame to perform all the 
// rendering calls for the scene, and it will also be called if the window needs to be 
// repainted. After this function has returned, the sample framework will call 
// IDirect3DDevice9::Present to display the contents of the next buffer in the swap chain
//--------------------------------------------------------------------------------------
void CALLBACK OnFrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext )
{
    // If the settings dialog is being shown, then
    // render it instead of rendering the app's scene
    if( g_SettingsDlg.IsActive() )
    {
        g_SettingsDlg.OnRender( DXUTGetElapsedTime() );
        return;
    }

    HRESULT hr;

	pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	
	// Begin the scene
    if (SUCCEEDED(pd3dDevice->BeginScene()))
    {		
		
		g_pHDREngine->onFrameRender(g_Camera.GetViewMatrix(), g_Camera.GetProjMatrix(), g_Camera.GetWorldMatrix(), g_Camera.GetEyePt(), fElapsedTime );

        // Render stats and help text  
        RenderText();

		V( g_HUD.OnRender( fElapsedTime ) );

        // End the scene.
        V( pd3dDevice->EndScene() );
	}
}


//--------------------------------------------------------------------------------------
// Toggle between full screen and windowed
//--------------------------------------------------------------------------------------
HRESULT HDRFP16x2_ToggleFullScreen()
{
    HRESULT hr;

	DXUTPause( true, true );
	
    // Get the current device settings and flip the windowed state then
    // find the closest valid device settings with this change
    DXUTDeviceSettings deviceSettings = DXUTGetDeviceSettings();

	// A few adjustment to avoid triple buffering and vsync on.
    deviceSettings.pp.Windowed             = !deviceSettings.pp.Windowed;
	deviceSettings.pp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	deviceSettings.pp.BackBufferCount      = 1;
	
    DXUTMatchOptions matchOptions;
    matchOptions.eAdapterOrdinal     = DXUTMT_PRESERVE_INPUT;
    matchOptions.eDeviceType         = DXUTMT_CLOSEST_TO_INPUT;
    matchOptions.eWindowed           = DXUTMT_PRESERVE_INPUT;
    matchOptions.eAdapterFormat      = DXUTMT_IGNORE_INPUT;
    matchOptions.eVertexProcessing   = DXUTMT_CLOSEST_TO_INPUT;
    matchOptions.eBackBufferFormat   = DXUTMT_IGNORE_INPUT;
    matchOptions.eBackBufferCount    = DXUTMT_PRESERVE_INPUT;
    matchOptions.eMultiSample        = DXUTMT_CLOSEST_TO_INPUT;
    matchOptions.eSwapEffect         = DXUTMT_CLOSEST_TO_INPUT;
    matchOptions.eDepthFormat        = DXUTMT_CLOSEST_TO_INPUT;
    matchOptions.eStencilFormat      = DXUTMT_CLOSEST_TO_INPUT;
    matchOptions.ePresentFlags       = DXUTMT_CLOSEST_TO_INPUT;
    matchOptions.eRefreshRate        = DXUTMT_IGNORE_INPUT;
    matchOptions.ePresentInterval    = DXUTMT_PRESERVE_INPUT;

	RECT rcWindowClient;
	if( deviceSettings.pp.Windowed )
	{
		// Going to widowed mode
        rcWindowClient = DXUTGetWindowClientRect();  // Get ol' window size
		int nWidth  = rcWindowClient.right  - rcWindowClient.left;
		int nHeight = rcWindowClient.bottom - rcWindowClient.top;
		if( nWidth > 0 && nHeight > 0 )
		{
			matchOptions.eResolution = DXUTMT_CLOSEST_TO_INPUT;
			deviceSettings.pp.BackBufferWidth  = nWidth;
			deviceSettings.pp.BackBufferHeight = nHeight;
		}
		else
		{
			matchOptions.eResolution = DXUTMT_IGNORE_INPUT;
		}
	}
	else{
        // Hack to force DXUT to fetch the desktop resolution
		matchOptions.eResolution = DXUTMT_IGNORE_INPUT; // rcWindowClient.right = rcWindowClient.left = rcWindowClient.bottom = rcWindowClient.top = 0; 
	}
 
	// Now let DXUT to do its thing
    hr = DXUTFindValidDeviceSettings( &deviceSettings, &deviceSettings, &matchOptions );
    if( SUCCEEDED(hr) ) 
    {
        // Create a Direct3D device using the new device settings.  
        // If there is an existing device, then it will either reset or recreate the scene.
        hr = DXUTCreateDeviceFromSettings( &deviceSettings, false );
    }

    DXUTPause( false, false );

    return hr;
}
