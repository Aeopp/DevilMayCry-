#ifndef __ENGINE_EXTERN_H__
#define __ENGINE_EXTERN_H__

extern ENGINE_DLL HINSTANCE				g_hInstance;
extern ENGINE_DLL HWND					g_hWnd;
extern ENGINE_DLL int					g_nWndCX;
extern ENGINE_DLL int					g_nWndCY;
extern ENGINE_DLL bool					g_bWindowed;
extern ENGINE_DLL bool                  g_bDebugMode;
extern ENGINE_DLL bool                  g_bEditMode;
extern ENGINE_DLL bool                  g_bCollisionVisible;
extern ENGINE_DLL bool                  g_bRenderTargetVisible;
extern ENGINE_DLL LPDIRECT3D9			g_pSDK;
extern ENGINE_DLL LPDIRECT3DDEVICE9		g_pDevice;
#endif // !__ENGINE_EXTERN_H__