#ifndef __UI_BTL_PANEL__
#define __UI_BTL_PANEL__
#include "GameObject.h"
#include "RenderInterface.h"

class BtlPanel : public ENGINE::GameObject,
				public ENGINE::RenderInterface				
{
private:
	enum UI_DESC_ID
	{
		REDORB = 0,
		TARGET_CURSOR,
		TARGET_HP,
		BOSS_GUAGE,
		HP_GLASS,
		EX_GAUGE,
		DESC_END
	};
	struct UI_DESC
	{
		bool	Using;
		Vector2	Pos;
		Vector2 Scale;
	};
	UI_DESC* _UIDescs = nullptr;

	std::shared_ptr<ENGINE::StaticMesh> _PlaneMesh{};
	std::shared_ptr<ENGINE::StaticMesh> _Pipe0Mesh{};
	std::shared_ptr<ENGINE::StaticMesh> _HPGlassMesh{};

	std::shared_ptr<ENGINE::Texture> _NoiseTex{};

	std::shared_ptr<ENGINE::Texture> _RedOrbALBMTex{};
	std::shared_ptr<ENGINE::Texture> _RedOrbATOSTex{};
	std::shared_ptr<ENGINE::Texture> _RedOrbNRMRTex{};

	std::shared_ptr<ENGINE::Texture> _TargetCursorTex{};
	std::shared_ptr<ENGINE::Texture> _EnemyHPTex{};

	std::shared_ptr<ENGINE::Texture> _BossGaugeATOSTex{};
	std::shared_ptr<ENGINE::Texture> _BossGaugeNRMRTex{};

	std::shared_ptr<ENGINE::Texture> _HPGlassATOSTex{};
	std::shared_ptr<ENGINE::Texture> _HPGlassNRMRTex{};
	std::shared_ptr<ENGINE::Texture> _GlassTex{};
	std::shared_ptr<ENGINE::Texture> _BloodTex{};

	std::shared_ptr<ENGINE::Texture> _EXBackALBMTex{};
	std::shared_ptr<ENGINE::Texture> _EXBackATOSTex{};
	std::shared_ptr<ENGINE::Texture> _EXBackNRMRTex{};

	float _AccumulateTime = 0.f;
	float _TotalAccumulateTime = 0.f;

	Vector3 _TargetPos = Vector3(0.f, 0.f, 0.f);
	float _TargetHP_Degree = 0.f;
	Vector2 _TargetHP_StartPtOrtho = Vector2(0.f, 0.f);
	Vector2 _TargetHP_Normal0 = Vector2(0.f, 0.f);
	Vector2 _TargetHP_Normal1 = Vector2(0.f, 0.f);
	
	float _BossGauge_CurXPosOrtho = 0.f;
	
	float _HPGlassDirt = 0.f;

	Vector3 _LightDir = Vector3(-1.f, 1.f, 1.f);
	
private:
	explicit BtlPanel() = default;
	virtual ~BtlPanel() = default;
	// GameObject을(를) 통해 상속됨
	virtual void Free() override;
	virtual std::string GetName() override;
	///
	void	Init_UIDescs();
	void	Create_ScreenMat(UI_DESC_ID _ID, Matrix& _Out, int _Opt = 0);
	void	Update_TargetInfo();
	Vector2	ScreenPosToOrtho(float _ScreenPosX, float _ScreenPosY);
	void	Imgui_ModifyUI(UI_DESC_ID _ID);
public:
	static BtlPanel* Create();
public:
	virtual void    RenderUIImplementation(const ImplementationInfo&_ImplInfo) override;
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
#endif // !__UI_BTL_PANEL__