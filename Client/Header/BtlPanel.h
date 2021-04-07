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
	std::shared_ptr<ENGINE::StaticMesh> _Pipe1Mesh{};

	std::shared_ptr<ENGINE::Texture> _NoiseTex{};

	std::shared_ptr<ENGINE::Texture> _RedOrbALBMTex{};
	std::shared_ptr<ENGINE::Texture> _RedOrbATOSTex{};
	std::shared_ptr<ENGINE::Texture> _RedOrbNRMRTex{};

	std::shared_ptr<ENGINE::Texture> _TargetCursorTex{};
	std::shared_ptr<ENGINE::Texture> _EnemyHPTex{};

	float _AccumulateTime = 0.f;
	Vector3 _TargetCursorPos = Vector3(640.f, 360.f, 0.f);

private:
	explicit BtlPanel() = default;
	virtual ~BtlPanel() = default;
	// GameObject을(를) 통해 상속됨
	virtual void Free() override;
	virtual std::string GetName() override;
	///
	void	Init_UIDescs();
	void	Create_ScreenMat(UI_DESC_ID _ID, Matrix& _Out, int _Opt = 0);
	void	Imgui_Modify_UIPosAndScale(UI_DESC_ID _ID);
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