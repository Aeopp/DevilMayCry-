#ifndef __MAPTOOL_SCENE_H__
#define __MAPTOOL_SCENE_H__
#include "Scene.h"
typedef D3DXVECTOR3 vec3;

#define MIN(A, B)            (((A) < (B)) ? (A) : (B))
#define MAX(A, B)            (((A) >= (B)) ? (A) : (B))
#define CLAMP(V, MN, MX)     ((V) < (MN) ? (MN) : (V) > (MX) ? (MX) : (V))
#define vZero vec3(0,0,0)



class MapTool final :  public ENGINE::Scene
{
	enum class ePropsOption { Decoration, Floating, Fixed, End };
	enum class eWorkOption { Create, Delete, Modify, End };
	enum class eCreatePosition {PeekingPos,PivotPos,End};

	typedef struct PropsPathInfo
	{
		PropsPathInfo()
			:sFileName(L"")
			, sFileLocation(L"")
		{
		}
		std::wstring GetFilePath()
		{
			return sFileLocation + L"/"+sFileName;
		}
		std::wstring sFileName; // 
		std::wstring sFileLocation;//파일 위치 

	}PATHINFO;
	enum  class ePeekingType
	{
		Single,Multi,End
	};



private:
	explicit MapTool();
	virtual ~MapTool() = default;
	virtual void Free() override;

private:
	float					m_fPivotMoveSpeed; //P
	int						m_iPeekType;
	int						m_iPeekCnt;
	bool					m_bPropsOption[(int)ePropsOption::End];
	std::string				m_strSaveFileName;
	eWorkOption				m_eWorkType;
	eCreatePosition			m_eCreateOption;
	bool					m_bReadyNameTable;				// 테이블 데이터 준비 
	std::unordered_map<size_t, PATHINFO> m_mapFBXNameTable; // 벨류값은 fbx파일 공통경로를 제외한 /StageN/Test.fbx이런식으로 
	
	size_t															 m_iTableID = 0;
	std::unordered_map<size_t, std::list<std::weak_ptr<GameObject>>> m_mapObjDatas;


private:
	void			ShowMapTool();
	void			ShowPivotControl();
	void			PivotControl();
	void			MouseInPut();

	void			CreateMeshNameTable(std::wstring strStartPath);
	void			HelpMarker(const char*	esc);
	bool			CheckWindow(const char* Text);
	void			HotKey();//단축키 모음 


	void			NameTableGroup();
	void			BaseMapCreateGroup();
	void			PeekingOptionGroup();
	void			TransFormCtrlGroup();
	void			PropsOptionGroup();
	bool			NewFBXNameTable(const _TCHAR* pPath);//파일 읽어서 새로운 파일 테이블 생성 
	bool			LoadFBXnametable(const _TCHAR* pPath);//파일 읽어서 

	void			SaveLoadingInfo();//이게 스테이지에 로딩할거 목록 
	void			SaveLoadData();//행렬정보등 저장 

	void			ApplyPropsOption();
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