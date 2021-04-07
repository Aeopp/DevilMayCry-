#ifndef __MAPTOOL_SCENE_H__
#define __MAPTOOL_SCENE_H__
#include "Scene.h"

#include "MapToolPivot.h"
#include "MapToolProps.h"


#define LOCATIONMESHPATH "../../Resource/Mesh/Map/Location/"
#define SAVEDATAPATH "../../SaveFile/SaveData/"
#define PROPSPATH L"../../Resource/Mesh/Map/Props"

const UINT nFileNameMaxLen = 512;

//Destory 호출 시 컨버트용
#define ConvertGameObjPtr(x) static_cast<std::weak_ptr<GameObject>>(x)  

#define CLAMP(V, MN, MX)     ((V) < (MN) ? (MN) : (V) > (MX) ? (MX) : (V))
#define vZero Vector3(0,0,0)
#define ERR_ID -1


class MapTool final : public ENGINE::Scene
{
	enum class eWorkOption { Create, Delete, Modify, End };
	enum class eCreatePosition { PeekingPos, PivotPos, End };
	enum  class ePeekingType { Single, Multi, End };
	enum  class eWindowID { MapTool, Pivot, Camera, End };
	typedef struct PropsPathInfo
	{
		PropsPathInfo()
			:sFileName(L"")
			, sFileLocation(L"")
		{
		}
		std::wstring GetFilePath()
		{
			return sFileLocation + L"/" + sFileName;
		}
		//FBX  이름 
		std::wstring sFileName;  
		//FBx 파일까지의 경로  상대경로임 
		std::wstring sFileLocation;

	}PATHINFO;
private:
	explicit MapTool();
	virtual ~MapTool() = default;
	virtual void Free() override;

private:

	// 피킹 함수에서 이거 체크 까먹지 말기  
	bool															m_bHoveredMaptool[(int)eWindowID::End];
	bool							                                m_bPropsOption[(int)MapToolProps::ePropsOption::End];
	std::wstring						                            m_strSelectFBXName; // 선택한 fbx 파일 이름 
	std::wstring						                            m_strPeekingName;  // 수정모드에서 선택한 파일 이름 

	ePeekingType							                        m_ePeekType;
	eWorkOption						                                m_eWorkType;
	eCreatePosition					                                m_eCreateOption;

	int								                                m_iPeekingCnt;			//멀티피킹시 피킹한  오브젝트 수 
	bool							                                m_bReadyNameTable;		// 테이블 데이터 준비체크 
	bool															m_bCreateLock = false;  // 생성 조건 부족시 블락처리용
	int																m_iTableID = ERR_ID;    // 이 아이디로 오브젝트 생성 할때 테이블의 키값으로 사용해서 매쉬 경로 값을 오브젝트한테 넘김 

	std::unordered_map<UINT, PATHINFO>                              m_mapFBXNameTable; // 벨류값은 fbx파일 공통경로를 제외한 /StageN/Test.fbx이런식으로 

	// 여기서 저장할  pros 종류랑 종류별로 파싱할 정보를 저장함  
	// 키값 불러오면 table에서 상대경로 받아와서 저장하고 pros 접근해서 정보 저장하고 
	std::unordered_map<UINT, std::list<std::weak_ptr<MapToolProps>>>  m_mapObjDatas;

	Matrix															m_matCameraWorld;
	Matrix															m_matView;
	Matrix															m_matProj;
	float															m_fFOV;
	Vector3															m_vCameraPos;
	Vector3															m_vRot;
	float							                                m_fPivotMoveSpeed; //P
	Vector2															m_vCameraSpeed; // x =  x z  , y = y 
	float															m_fCameraAngSpeed;

	std::weak_ptr<MapToolProps>										m_pBaseMap ; //마우스 피킹대상 
	std::weak_ptr<MapToolProps>										m_pCurSelectObj; // 싱글타입 피킹일대 현재 선택한 오브젝트 
	std::list<std::weak_ptr<MapToolProps>>							m_listMultiPeeking; // 멀티피킹일떄 피킹 대상들 담을 녀석 
	std::weak_ptr<MapToolPivot>										m_pPivot;

private:

	void															HotKey();//단축키 모음 
	void															PivotControl(const float& fDeltaTime);
	void															MousePeeking();
	void															UpdateProj(); //프로젝션 값 수정시 호출 
	void															UpdateView(); //뷰스페이스 수정시 호출 
	void															CameraControl(const float& _fDeltaTime);

	void															SelectFile();
	void															CreateProps(); // 오브젝트 Create 시 호출 
	void															ClearMultiPeeking(); //피킹 타입 바뀌거나 Workoption 변경시 멀티피킹리스트 초기화 처리 
	void															ApplyPropsOption();  //오브젝트 속성값 수정시 호출 그냥 콤보박스 누르면 호출됨 
	
	//Save Load
	HRESULT															LoadBaseMap(); 
	HRESULT															CreateMeshNameTable(std::wstring strStartPath); //파일 탐색하면서 NameTable 세팅 
	bool															NewFBXNameTable(const _TCHAR* pPath);//리소스 파일 탐색하면서 네임테이블 세팅 및 결과값 json으로 저장 
	bool															LoadFBXnametable(const _TCHAR* pPath);//저장한 json 파일로 nameTable 세팅 
	void															SaveProps();// 저장 

	//Gui 정리
	void															ShowMapTool();
	void															ShowPivotOption();
	void															ShowCameraOption();

	void															HelpMarker(const char* esc);
	//Goupe
	void															NameTableGroup();
	void															BaseMapCreateGroup();
	void															PeekingOptionGroup();
	void															TransFormCtrlGroup();
	void															PropsOptionGroup();
	void															SaveFileGroup();

	//마우스가 UI 위에 있을 경우 피킹 블락용으로 쓸거임 혹은  
	//이전 버전에서 타이핑 칠때  wads 가 카메라 조작키이니 이걸로 카메라 무브도 막았음 타이핑 필요하면 이거 호출 
	bool									IsHoverUIWindow(); 

	std::wstring							convertToWstring(const std::string& s);
	std::string								convertToString(const std::wstring& s);

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