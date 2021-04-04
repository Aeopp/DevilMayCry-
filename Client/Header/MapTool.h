#ifndef __MAPTOOL_SCENE_H__
#define __MAPTOOL_SCENE_H__
#include "Scene.h"
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
		std::wstring sFileLocation;//���� ��ġ 

	}PATHINFO;




private:
	explicit MapTool();
	virtual ~MapTool() = default;
	virtual void Free() override;

private:

	// ��ŷ �Լ����� �̰� üũ ����� ����  
	bool															m_bHoveredMaptool[(int)eWindowID::End];
	ePeekingType							                        m_ePeekType;
	bool							                                m_bPropsOption[(int)ePropsOption::End];
	std::wstring						                            m_strSelectName;
	eWorkOption						                                m_eWorkType;
	eCreatePosition					                                m_eCreateOption;

	float							                                m_fPivotMoveSpeed; //P
	int								                                m_iPeekingCnt;			//��Ƽ�ɼǽ� ��ŷ�� ������Ʈ �� 
	bool							                                m_bReadyNameTable;			  // ���̺� ������ �غ� 
	std::unordered_map<UINT, PATHINFO>                              m_mapFBXNameTable; // �������� fbx���� �����θ� ������ /StageN/Test.fbx�̷������� 

	int																m_iTableID = ERR_ID;
	std::unordered_map<UINT, std::list<std::weak_ptr<GameObject>>>  m_mapObjDatas;

	Matrix															m_matCameraWorld;
	Matrix															m_matView;
	Matrix															m_matProj;
	float															m_fFOV;
	Vector3															m_vCameraPos;
	Vector3															m_vRot;
	Vector2															m_vCameraSpeed; // x =  x z  , y = y 
	float															m_fCameraAngSpeed;

private:
	void			ShowMapTool();
	void			ShowPivotControl();
	void			PivotControl();
	void			MouseInPut();

	void			CreateMeshNameTable(std::wstring strStartPath);
	void			HelpMarker(const char*	esc);
	bool			CheckWindow(const char* Text);
	void			HotKey();//����Ű ���� 

	bool			NewFBXNameTable(const _TCHAR* pPath);//���� �о ���ο� ���� ���̺� ���� 
	bool			LoadFBXnametable(const _TCHAR* pPath);//���� �о 

	void			SaveLoadingList(const std::string& pPath);//�̰� ���������� �ε��Ұ� ��� 
	void			SaveObjInfo(const std::string& pPath);//��������� ���� 

	void			ApplyPropsOption();  //������Ʈ �Ӽ��� ������ ȣ�� �׳� �޺��ڽ� ������ ȣ��� 
	//Camera
	void			UpdateProj(); //�������� �� ������ ȣ�� 
	void			UpdateView(); //�佺���̽� ������ ȣ�� 
	void			CameraControl(const float& _fDeltaTime);

	//Gui ����
	void			NameTableGroup();
	void			BaseMapCreateGroup();
	void			PeekingOptionGroup();
	void			TransFormCtrlGroup();
	void			PropsOptionGroup();
	void			SaveFileGroup();


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