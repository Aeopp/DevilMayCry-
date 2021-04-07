#ifndef __MAPTOOL_SCENE_H__
#define __MAPTOOL_SCENE_H__
#include "Scene.h"

#include "MapToolPivot.h"
#include "MapToolProps.h"


#define LOCATIONMESHPATH "../../Resource/Mesh/Map/Location/"
#define SAVEDATAPATH "../../SaveFile/SaveData/"
#define PROPSPATH L"../../Resource/Mesh/Map/Props"

const UINT nFileNameMaxLen = 512;

//Destory ȣ�� �� ����Ʈ��
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
		//FBX  �̸� 
		std::wstring sFileName;  
		//FBx ���ϱ����� ���  ������� 
		std::wstring sFileLocation;

	}PATHINFO;
private:
	explicit MapTool();
	virtual ~MapTool() = default;
	virtual void Free() override;

private:

	// ��ŷ �Լ����� �̰� üũ ����� ����  
	bool															m_bHoveredMaptool[(int)eWindowID::End];
	bool							                                m_bPropsOption[(int)MapToolProps::ePropsOption::End];
	std::wstring						                            m_strSelectFBXName; // ������ fbx ���� �̸� 
	std::wstring						                            m_strPeekingName;  // ������忡�� ������ ���� �̸� 

	ePeekingType							                        m_ePeekType;
	eWorkOption						                                m_eWorkType;
	eCreatePosition					                                m_eCreateOption;

	int								                                m_iPeekingCnt;			//��Ƽ��ŷ�� ��ŷ��  ������Ʈ �� 
	bool							                                m_bReadyNameTable;		// ���̺� ������ �غ�üũ 
	bool															m_bCreateLock = false;  // ���� ���� ������ ���ó����
	int																m_iTableID = ERR_ID;    // �� ���̵�� ������Ʈ ���� �Ҷ� ���̺��� Ű������ ����ؼ� �Ž� ��� ���� ������Ʈ���� �ѱ� 

	std::unordered_map<UINT, PATHINFO>                              m_mapFBXNameTable; // �������� fbx���� �����θ� ������ /StageN/Test.fbx�̷������� 

	// ���⼭ ������  pros ������ �������� �Ľ��� ������ ������  
	// Ű�� �ҷ����� table���� ����� �޾ƿͼ� �����ϰ� pros �����ؼ� ���� �����ϰ� 
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

	std::weak_ptr<MapToolProps>										m_pBaseMap ; //���콺 ��ŷ��� 
	std::weak_ptr<MapToolProps>										m_pCurSelectObj; // �̱�Ÿ�� ��ŷ�ϴ� ���� ������ ������Ʈ 
	std::list<std::weak_ptr<MapToolProps>>							m_listMultiPeeking; // ��Ƽ��ŷ�ϋ� ��ŷ ���� ���� �༮ 
	std::weak_ptr<MapToolPivot>										m_pPivot;

private:

	void															HotKey();//����Ű ���� 
	void															PivotControl(const float& fDeltaTime);
	void															MousePeeking();
	void															UpdateProj(); //�������� �� ������ ȣ�� 
	void															UpdateView(); //�佺���̽� ������ ȣ�� 
	void															CameraControl(const float& _fDeltaTime);

	void															SelectFile();
	void															CreateProps(); // ������Ʈ Create �� ȣ�� 
	void															ClearMultiPeeking(); //��ŷ Ÿ�� �ٲ�ų� Workoption ����� ��Ƽ��ŷ����Ʈ �ʱ�ȭ ó�� 
	void															ApplyPropsOption();  //������Ʈ �Ӽ��� ������ ȣ�� �׳� �޺��ڽ� ������ ȣ��� 
	
	//Save Load
	HRESULT															LoadBaseMap(); 
	HRESULT															CreateMeshNameTable(std::wstring strStartPath); //���� Ž���ϸ鼭 NameTable ���� 
	bool															NewFBXNameTable(const _TCHAR* pPath);//���ҽ� ���� Ž���ϸ鼭 �������̺� ���� �� ����� json���� ���� 
	bool															LoadFBXnametable(const _TCHAR* pPath);//������ json ���Ϸ� nameTable ���� 
	void															SaveProps();// ���� 

	//Gui ����
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

	//���콺�� UI ���� ���� ��� ��ŷ ��������� ������ Ȥ��  
	//���� �������� Ÿ���� ĥ��  wads �� ī�޶� ����Ű�̴� �̰ɷ� ī�޶� ���굵 ������ Ÿ���� �ʿ��ϸ� �̰� ȣ�� 
	bool									IsHoverUIWindow(); 

	std::wstring							convertToWstring(const std::string& s);
	std::string								convertToString(const std::wstring& s);

public:
	static MapTool* Create();
public:
	// Scene��(��) ���� ��ӵ�
	virtual HRESULT LoadScene() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual HRESULT Update(const float _fDeltaTime) override;
	virtual HRESULT LateUpdate(const float _fDeltaTime) override;
};



#endif // !__MAPTOOL_SCENE_H__