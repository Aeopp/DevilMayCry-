#ifndef __MAPTOOL_SCENE_H__
#define __MAPTOOL_SCENE_H__
#include "Scene.h"

#include "MapToolObj.h"
#include "MapToolProps.h"
#define LOCATIONMESHPATH "../../Resource/Map/Location/"
#define SAVEDATAPATH "../../Resource/SaveData/"
#define PROPSPATH "../../Resource/Map/Props/"

const UINT nFileNameMaxLen = 512;
#define ConvertGameObjPtr(x) static_cast<std::weak_ptr<GameObject>>(x)
typedef D3DXVECTOR3 vec3;

#define MIN(A, B)            (((A) < (B)) ? (A) : (B))
#define MAX(A, B)            (((A) >= (B)) ? (A) : (B))
#define CLAMP(V, MN, MX)     ((V) < (MN) ? (MN) : (V) > (MX) ? (MX) : (V))
#define vZero vec3(0,0,0)
#define ERR_ID -1


class MapTool final : public ENGINE::Scene
{
	enum class ePropsOption { Decoration, Floating, Fixed, End };
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
		std::wstring sFileName; // 
		std::wstring sFileLocation;//파일 위치 

	}PATHINFO;




private:
	explicit MapTool();
	virtual ~MapTool() = default;
	virtual void Free() override;

private:

	// 피킹 함수에서 이거 체크 까먹지 말기  
	bool															m_bHoveredMaptool[(int)eWindowID::End];
	ePeekingType							                        m_ePeekType;
	bool							                                m_bPropsOption[(int)ePropsOption::End];
	std::wstring						                            m_strSelectName;
	std::wstring						                            m_strSelectFilePath;
	eWorkOption						                                m_eWorkType;
	eCreatePosition					                                m_eCreateOption;

	float							                                m_fPivotMoveSpeed; //P
	int								                                m_iPeekingCnt;			//멀티옵션시 피킹한 오브젝트 수 
	bool							                                m_bReadyNameTable;			  // 테이블 데이터 준비 

	int																m_iTableID = ERR_ID;

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
	Vector2															m_vCameraSpeed; // x =  x z  , y = y 
	float															m_fCameraAngSpeed;

	std::weak_ptr<MapToolProps>										m_pBaseMap ;
	std::weak_ptr<MapToolProps>										m_pCurSelectObj;
	std::weak_ptr<MapToolObj>										m_pPivot;

private:

	void			PivotControl(const float& fDeltaTime);
	void			MouseInPut();

	HRESULT			CreateMeshNameTable(std::wstring strStartPath);
	void			HelpMarker(const char*	esc);
	bool			CheckWindow(const char* Text);
	void			HotKey();//단축키 모음 
	 
	bool			ObjKeyFinder(const _TCHAR* pTag);//키중복 확인 Haskey .
	//Base map
	HRESULT			LoadBaseMap(std::wstring strFilePath);

	bool			NewFBXNameTable(const _TCHAR* pPath);//파일 읽어서 새로운 파일 테이블 생성 
	bool			LoadFBXnametable(const _TCHAR* pPath);//파일 읽어서 

	//add & Select
	void			AddProps(const _TCHAR* pPath,const _TCHAR* pName);
	void			SelectFile();

	//Save
	void			SaveLoadingList(const std::string& pPath);//이게 스테이지에 로딩할거 목록 
	void			SaveProps(const std::string& pPath);// 소품 저장 

	void			ApplyPropsOption();  //오브젝트 속성값 수정시 호출 그냥 콤보박스 누르면 호출됨 
	//Camera
	void			UpdateProj(); //프로젝션 값 수정시 호출 
	void			UpdateView(); //뷰스페이스 수정시 호출 
	void			CameraControl(const float& _fDeltaTime);

	//Gui 정리
	void			ShowMapTool();
	void			ShowPivotOption();
	void			ShowCameraOption();

	void			NameTableGroup();
	void			BaseMapCreateGroup();
	void			PeekingOptionGroup();
	void			TransFormCtrlGroup();
	void			PropsOptionGroup();
	void			SaveFileGroup();

	bool			IsHoverUIWindow();

	std::wstring		convertToWstring(const std::string& s);
	std::string			convertToString(const std::wstring& s);

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