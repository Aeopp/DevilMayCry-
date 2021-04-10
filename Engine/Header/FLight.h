//#ifndef _FLIGHT_H_
//
//#define _FLIGHT_H_
//#include "EngineDefine.h"
//#include "EngineTypedef.h"
//#include <functional>
//
//BEGIN(ENGINE)
//
//class ENGINE_DLL FLight
//{
//public:
//	enum Type : uint8 
//	{
//		Point = 0,
//		Directional,
//		Spot,
//	};
//private:
//	D3DXVECTOR4				position;	// or direction
//	D3DXVECTOR4				projparams;
//	D3DXVECTOR3				spotdirection;
//	D3DXVECTOR2				spotparams;	// cos(inner), cos(outer)
//
//	D3DXCOLOR				color;
//	LPDIRECT3DCUBETEXTURE9	cubeshadowmap;
//	LPDIRECT3DCUBETEXTURE9	blurredcubeshadowmap;
//	LPDIRECT3DTEXTURE9		shadowmap;
//	LPDIRECT3DTEXTURE9		blurredshadowmap;
//	Type				    type;
//	int						currentface;
//	uint16_t				shadowmapsize;
//	bool					blurred;
//
//public:
//	FLight(Type type, const D3DXVECTOR4& position, const D3DXCOLOR& color);
//	~FLight();
//
//	void CalculateViewProjection(D3DXMATRIX& out);
//	void CalculateScissorRect(RECT& out, const D3DXMATRIX& view, const D3DXMATRIX& proj, float radius, int32_t width, int32_t height);
//
//	void CreateShadowMap(LPDIRECT3DDEVICE9 device, uint16_t size);
//	void RenderShadowMap(LPDIRECT3DDEVICE9 device, std::function<void(FLight*)> callback);
//
//	// 다른 라이팅ㅇ으로부터 쉐오둥 블러 . 
//	void BlurShadowMap(LPDIRECT3DDEVICE9 device, std::function<void(FLight*)> callback);
//
//	// 사이즈 투영 near far 
//	void SetProjectionParameters(float xsize, float ysize, float znear, float zfar);
//	// 방향 inner outer cos 단위.
//	void SetSpotParameters(const D3DXVECTOR3& dir, float inner, float outer);
//
//	inline void SetPosition(const D3DXVECTOR4& newpos) { position = newpos; }
//
//	inline float GetNearPlane() const { return projparams.z; }
//	inline float GetFarPlane() const { return projparams.w; }
//	inline uint16_t GetShadowMapSize() const { return shadowmapsize; }
//	inline const D3DXVECTOR2& GetSpotParameters() const { return spotparams; }
//
//	inline D3DXVECTOR4& GetPosition() { return position; }
//	inline D3DXVECTOR3& GetSpotDirection() { return spotdirection; }
//	inline D3DXCOLOR& GetColor() { return color; }
//
//	inline LPDIRECT3DTEXTURE9 GetShadowMap() { return (blurred ? blurredshadowmap : shadowmap); }
//	inline LPDIRECT3DCUBETEXTURE9 GetCubeShadowMap() { return (blurred ? blurredcubeshadowmap : cubeshadowmap); }
//};
//
//END
//
//#endif // !_FLIGHT_H_
//
