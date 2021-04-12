#ifndef _FLIGHT_H_

#define _FLIGHT_H_
#include "EngineDefine.h"
#include "EngineTypedef.h"
#include <d3d9.h>
#include <d3dx9.h>
#include <functional>

BEGIN(ENGINE)

class ENGINE_DLL FLight
{
public:
	enum Type : uint8 
	{
		Point = 0,
		Directional,
		Spot,
	};
private:
	D3DXVECTOR4				Position;	// or direction
	D3DXVECTOR4				Projparams;
	D3DXVECTOR3				Spotdirection;
	D3DXVECTOR2				Spotparams;	// cos(inner), cos(outer)

	D3DXCOLOR				Color;
	LPDIRECT3DCUBETEXTURE9	Cubeshadowmap;
	LPDIRECT3DCUBETEXTURE9	Blurredcubeshadowmap;
	LPDIRECT3DTEXTURE9		Shadowmap;
	LPDIRECT3DTEXTURE9		Blurredshadowmap;
	Type				    _Type;
	int						Currentface;
	uint16_t				ShadowMapSize;
	bool					Blurred;

public:
	FLight(Type _Type, 
		const D3DXVECTOR4& position, const D3DXCOLOR& color);
	~FLight();

	void Edit(const uint32 Idx);

	void CalculateViewProjection(D3DXMATRIX& Out);
	void CalculateScissorRect(RECT& Out, const D3DXMATRIX& View, const D3DXMATRIX& Projection, float Radius, int32_t Width, int32_t Height);

	void CreateShadowMap(LPDIRECT3DDEVICE9 _Device, const uint16_t Size);
	void RenderShadowMap(LPDIRECT3DDEVICE9 _Device, std::function<void(FLight*)> CallBack);

	// �ٸ� �����ä����κ��� ������ ���� . 
	void BlurShadowMap(LPDIRECT3DDEVICE9 _Device, std::function<void(FLight*)> CallBack);

	// ������ ���� near far 
	void SetProjectionParameters(const float XSize, const float YSize, const float ZNear, const float ZFar);
	// ���� inner outer cos ����.
	void SetSpotParameters(const D3DXVECTOR3& Direction , const float Inner, const float Outer);

	inline void SetPosition(const D3DXVECTOR4& Newpos) { Position = Newpos; }

	inline float GetNearPlane() const { return Projparams.z; }
	inline float GetFarPlane() const { return Projparams.w; }
	inline uint16_t GetShadowMapSize() const { return ShadowMapSize; }
	inline const D3DXVECTOR2& GetSpotParameters() const { return Spotparams; }

	inline D3DXVECTOR4& GetPosition()     { return Position; }
	inline D3DXVECTOR3& GetSpotDirection() { return Spotdirection; }
	inline D3DXCOLOR& GetColor() { return Color; }

	inline bool IsPerspective()const& {return _Type!=Directional;};
	inline LPDIRECT3DTEXTURE9 GetShadowMap() { return (Blurred ? Blurredshadowmap : Shadowmap); }
	inline LPDIRECT3DCUBETEXTURE9 GetCubeShadowMap() { return (Blurred ? Blurredcubeshadowmap : Cubeshadowmap); }
};
END

#endif // !_FLIGHT_H_
