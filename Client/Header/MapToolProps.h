#pragma once
#ifndef __MAPTOOLPROPS_H__
#define __MAPTOOLPROPS_H__

#include "GameObject.h"
#include "RenderInterface.h"

class MapToolProps :public ENGINE::GameObject, public ENGINE::RenderInterface
{
public:
	enum class ePropsOption { Decoration, Floating, Fixed, End }; // 이후 계속 추가 
public:
	std::weak_ptr<Transform>		Get_Trans() { return GetComponent<ENGINE::Transform>(); };
	//void							SetFBXPath(const std::wstring& strPath) { m_strFilePath = strPath; } // 걍 public 박자  근대 
	std::wstring					m_strFilePath; //fbx 경로 load 할때 쓰는거 
	int								m_iPropsID; //테이블에서 키값 
	//bool 추가
	bool							m_bMultiPeekPeeking; // 멀티 피킹시 무조건 체크 중복 피킹 방지  
	bool							m_bOption[(int)ePropsOption::End];

	bool							Peeking(Vector3 vRayPos, Vector3 vRayDir, OUT Vector3 &PeekingPos); // 




	static MapToolProps* Create();
private: 
	std::shared_ptr<ENGINE::StaticMesh> _StaticMesh{};
private:
	explicit MapToolProps() = default;
	virtual ~MapToolProps() = default;
public:
	// GameObject을(를) 통해 상속됨
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