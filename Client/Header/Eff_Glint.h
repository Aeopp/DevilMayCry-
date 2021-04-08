#ifndef __EFF_GLINT__
#define __EFF_GLINT__
#include "GameObject.h"
#include "RenderInterface.h"

class Eff_Glint : public ENGINE::GameObject,
				public ENGINE::RenderInterface				
{
private:
	std::shared_ptr<ENGINE::StaticMesh> _PlaneMesh{};
	std::shared_ptr<ENGINE::StaticMesh> _SphereMesh{};

	std::shared_ptr<ENGINE::Texture> _GlintTex0{};
	std::shared_ptr<ENGINE::Texture> _GlintTex1{};

	float _AccumulateTime = 0.f;
	float _PlayingSpeed = 3.5f;
	float _SliceAmount = 0.f;
	float _AllScale = 0.f;
	UINT _GlintTexIdx = 0u;
	Matrix _SphereScaleMatrix = Matrix();

private:
	explicit Eff_Glint() = default;
	virtual ~Eff_Glint() = default;
	// GameObject을(를) 통해 상속됨
	virtual void Free() override;
	virtual std::string GetName() override;
public:
	static Eff_Glint* Create();
public:
	virtual void    RenderAlphaBlendEffectImplementation(const ImplementationInfo&_ImplInfo) override;
	virtual void    RenderReady() override;
public:
	virtual HRESULT Ready() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT    Update(const float _fDeltaTime) override;
	virtual UINT    LateUpdate(const float _fDeltaTime) override;
	virtual void    Editor() override;
	virtual void	OnEnable() override;
	virtual void    OnDisable() override;
};
#endif // !__EFF_GLINT__