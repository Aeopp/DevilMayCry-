#ifndef __EFF_OVERTUREHAND__
#define __EFF_OVERTUREHAND__
#include "GameObject.h"
#include "RenderInterface.h"

class Eff_OvertureHand : public ENGINE::GameObject,
						public ENGINE::RenderInterface				
{
private:
	std::shared_ptr<ENGINE::StaticMesh> _HandMesh{};
	std::shared_ptr<ENGINE::Texture> _LightningTex{};
	std::shared_ptr<ENGINE::Texture> _GlowTex{};
	std::shared_ptr<ENGINE::Texture> _LightningColorTex{};
	std::shared_ptr<ENGINE::Texture> _NoiseTex{};

	float _SliceAmount = 0.f;
	float _RandTexV0 = 0.8f;
	float _RandTexV1 = 0.6f;
	float _AccumulateTime = 0.f;
	float _PlayingSpeed = 1.f;

private:
	explicit Eff_OvertureHand() = default;
	virtual ~Eff_OvertureHand() = default;
	// GameObject을(를) 통해 상속됨
	virtual void Free() override;
	virtual std::string GetName() override;
public:
	static Eff_OvertureHand* Create();
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
#endif // !__EFF_OVERTUREHAND__