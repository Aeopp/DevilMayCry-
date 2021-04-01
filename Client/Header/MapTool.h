#ifndef __MAPTOOL_SCENE_H__
#define __MAPTOOL_SCENE_H__
#include "Scene.h"

typedef D3DXVECTOR3 vec3;

#define MIN(A, B)            (((A) < (B)) ? (A) : (B))
#define MAX(A, B)            (((A) >= (B)) ? (A) : (B))
#define CLAMP(V, MN, MX)     ((V) < (MN) ? (MN) : (V) > (MX) ? (MX) : (V))
#define vZero vec3(0,0,0)
namespace ePropsOption
{
	enum e { Decoration, Floating, Fixed,End};
};

class MapTool final :  public ENGINE::Scene
{
	enum  class ePeekingType
	{
		Single,Multi,End
	};


private:
	explicit MapTool();
	virtual ~MapTool() = default;
	virtual void Free() override;

private:
	float			m_fPivotMoveSpeed; //P
	int				m_iPeekType;
	int				m_iPeekCnt;
	bool			m_bPropsOptino[ePropsOption::End];
private:
	void	ShowMapTool();
	void	ShowPivotControl();
	void	PivotControl();
	void	MouseInPut();

	void    HelpMarker(const char* desc);
public:
	static MapTool* Create();
public:
	// Scene을(를) 통해 상속됨
	virtual HRESULT LoadScene() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual HRESULT Update(const float _fDeltaTime) override;
	virtual HRESULT LateUpdate(const float _fDeltaTime) override;
};


#endif // !__MAPTOOL_SCENE_H__