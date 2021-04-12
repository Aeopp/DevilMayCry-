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
		EX_GAUGE_BACK,
		EX_GAUGE,
		HP_GAUGE,
		TDT_GAUGE,
		KEYBOARD,
		DESC_END
	};
	struct UI_DESC
	{
		bool	Using;
		Vector3	Pos;	// z -> plane 기준 0.02보다 작으면 짤림
		Vector3 Scale;
	};
	UI_DESC* _UIDescs = nullptr;

	std::shared_ptr<ENGINE::StaticMesh> _PlaneMesh{};
	std::shared_ptr<ENGINE::StaticMesh> _Pipe0Mesh{};

	std::shared_ptr<ENGINE::Texture> _NoiseTex{};

	std::shared_ptr<ENGINE::Texture> _RedOrbALBMTex{};
	std::shared_ptr<ENGINE::Texture> _RedOrbATOSTex{};
	std::shared_ptr<ENGINE::Texture> _RedOrbNRMRTex{};

	std::shared_ptr<ENGINE::Texture> _TargetCursorTex{};
	std::shared_ptr<ENGINE::Texture> _EnemyHPTex{};

	std::shared_ptr<ENGINE::Texture> _BossGaugeATOSTex{};
	std::shared_ptr<ENGINE::Texture> _BossGaugeNRMRTex{};

	std::shared_ptr<ENGINE::StaticMesh> _HPGlassMesh{};
	std::shared_ptr<ENGINE::Texture> _HPGlassATOSTex{};
	std::shared_ptr<ENGINE::Texture> _HPGlassNRMRTex{};
	std::shared_ptr<ENGINE::Texture> _GlassTex{};
	std::shared_ptr<ENGINE::Texture> _BloodTex{};

	std::shared_ptr<ENGINE::StaticMesh> _Ex0Mesh{};
	std::shared_ptr<ENGINE::StaticMesh> _Ex1Mesh{};
	std::shared_ptr<ENGINE::StaticMesh> _Ex2Mesh{};
	std::shared_ptr<ENGINE::Texture> _ExBackALBMTex{};
	std::shared_ptr<ENGINE::Texture> _ExBackATOSTex{};
	std::shared_ptr<ENGINE::Texture> _ExBackNRMRTex{};
	std::shared_ptr<ENGINE::Texture> _ExALBM0Tex{};
	std::shared_ptr<ENGINE::Texture> _ExNRMR0Tex{};
	std::shared_ptr<ENGINE::Texture> _ExALBM1Tex{};
	std::shared_ptr<ENGINE::Texture> _ExNRMR1Tex{};

	std::shared_ptr<ENGINE::Texture> _HPGaugeBaseALBMTex{};
	std::shared_ptr<ENGINE::Texture> _HPGaugeBaseATOSTex{};
	std::shared_ptr<ENGINE::Texture> _HPGaugeBaseNRMRTex{};
	std::shared_ptr<ENGINE::Texture> _HPGaugeALBMTex{};
	std::shared_ptr<ENGINE::Texture> _HPGaugeATOSTex{};
	std::shared_ptr<ENGINE::Texture> _HPGaugeNRMRTex{};

	std::shared_ptr<ENGINE::Texture> _TDTGaugeATOSTex{};
	std::shared_ptr<ENGINE::Texture> _TDTGaugeNRMRTex{};

	std::shared_ptr<ENGINE::Texture> _KeyBoardTex{};

	float _AccumulateTime = 0.f;
	float _TotalAccumulateTime = 0.f;

	Vector3 _TargetPos = Vector3(0.f, 0.f, 0.f);
	float _TargetHP_Degree = 0.f;
	Vector2 _TargetHP_StartPtOrtho = Vector2(0.f, 0.f);
	Vector2 _TargetHP_Normal0 = Vector2(0.f, 0.f);
	Vector2 _TargetHP_Normal1 = Vector2(0.f, 0.f);
	
	int _HPGaugeCount = 6;
	float _HPGaugeWidth = 50.f;
	float _HPGauge_CurXPosOrtho = 0.f;
	float _BossGauge_CurXPosOrtho = 0.f;
	float _TDTGauge_CurXPosOrtho = 0.f;

	float _HPGlassDirt = 0.f;

	enum KEY_INPUT_ID
	{
		Q = 0, W, E, A, S, D, F, Z,
		SHIFT, CTRL, SPACE, LBUTTON, MBUTTON, RBUTTON,
		KEY_INPUT_END
	};
	bool _KeyboardInput[KEY_INPUT_END] = { false, };

	Matrix _PerspectiveProjMatrix = Matrix();

	Vector3 _LightDir = Vector3(0.f, 1.f, 1.f);

	Vector3 _Rot = Vector3(0.f, 0.f, 0.f);	// 디버그용 회전벡터(Degree). 회전값이 들어간게 예외케이스라 생각해서 UI_DESC에 없음
	Vector2 _MinTexUV = Vector2(0.f, 0.f);
	Vector2 _MaxTexUV = Vector2(1.f, 1.f);

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
	Vector2	WorldPosToScreenPos(const Vector3& WorldPos);
	Vector2	ScreenPosToOrtho(float _ScreenPosX, float _ScreenPosY);
	void	Check_KeyInput();
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
public:
	void SetTargetActive(bool IsActive);
	void SetTargetPos(const Vector3& pos) { _TargetPos = pos; }
	void SetKeyInputActive(bool IsActive);
};
#endif // !__UI_BTL_PANEL__