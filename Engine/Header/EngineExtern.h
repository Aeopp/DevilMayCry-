#ifndef __ENGINE_EXTERN_H__
#define __ENGINE_EXTERN_H__

extern ENGINE_DLL HINSTANCE				g_hInstance;
extern ENGINE_DLL HWND					g_hWnd;
extern ENGINE_DLL int					g_nWndCX;
extern ENGINE_DLL int					g_nWndCY;
extern ENGINE_DLL ENGINE::WNDMODE				g_eWndMode;
extern ENGINE_DLL bool                  bDebugMode;
extern ENGINE_DLL bool                  bEditMode;
extern ENGINE_DLL bool                  bDebugCollision;
extern ENGINE_DLL bool                  bDebugRenderTargetRender;

#endif // !__ENGINE_EXTERN_H__