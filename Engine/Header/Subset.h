#ifndef __SUBSET_H__
#define __SUBSET_H__

#include "Object.h"
#include "Material.h"
#include "VertexInformation.hpp"

BEGIN(ENGINE)
class ENGINE_DLL Subset final : public Object
{
public : 
	VertexInformation _VertexInformation{};
	Material _Material {};
private:
	LPDIRECT3DDEVICE9	         _Device{ nullptr };
private:
	explicit Subset(LPDIRECT3DDEVICE9 const _pDevice);
	// Object을(를) 통해 상속됨
public:
	virtual ~Subset() = default;
	virtual void Free() override;
public:
	static Subset* Create(LPDIRECT3DDEVICE9 const _pDevice);

	void Initialize(
		const VertexInformation& InitVertexInformation , 
		const Material& InitMaterial);
};
END
#endif // !__SUBSET_H__