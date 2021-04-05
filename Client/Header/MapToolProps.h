#pragma once
#ifndef  __MAPTOOLPROPS_H__
#define __MAPTOOLPROPS_H__

#include "GameObject.h"
#include "RenderInterface.h"

class MapToolProps :public ENGINE::GameObject, public ENGINE::RenderInterface
{
public:
	enum class ePropsOption { Decoration, Floating, Fixed, End };
public:
	std::weak_ptr<Transform>		Get_Trans() { return GetComponent<ENGINE::Transform>(); };
	void							SetFBXPath(const std::wstring& strPath) { m_strFilePath = strPath; }
	std::wstring					m_strFilePath; //fbx ���
	int								m_iPropsID; //���̺��� Ű�� 
	//bool �߰�
	bool							m_bMultiPeekPeeking; // ��Ƽ ��ŷ�� ������ üũ �ߺ� ��ŷ ����  
	bool							m_bOption[(int)ePropsOption::End];
	static MapToolProps* Create();
private: 
	std::shared_ptr<ENGINE::StaticMesh> _StaticMesh{};
private:
	explicit MapToolProps() = default;
	virtual ~MapToolProps() = default;
public:
	// GameObject��(��) ���� ��ӵ�
	virtual void Free() override;
	virtual HRESULT Ready() override;
	virtual HRESULT Awake() override;
	virtual HRESULT Start() override;
	virtual UINT Update(const float _fDeltaTime) override;
	virtual UINT LateUpdate(const float _fDeltaTime) override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;

	virtual void RenderForwardAlphaBlendImplementation(const ImplementationInfo& _ImplInfo) override;
	virtual void RenderGBufferImplementation(const ImplementationInfo& _ImplInfo)override;
	virtual void RenderReady() override;

};


#endif // ! __Props