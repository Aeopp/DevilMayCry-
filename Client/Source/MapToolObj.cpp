#include "stdafx.h"
#include "..\Header\MapToolObj.h"
void MapToolObj::Free()
{
}

MapToolObj* MapToolObj::Create()
{
    return new MapToolObj{};
}

HRESULT MapToolObj::Ready()
{
    return S_OK;
}

HRESULT MapToolObj::Awake()
{
    return S_OK;
}

HRESULT MapToolObj::Start()
{
    return S_OK;
}

UINT MapToolObj::Update(const float _fDeltaTime)
{
    return 0;
}

UINT MapToolObj::LateUpdate(const float _fDeltaTime)
{
    return 0;
}

void MapToolObj::OnEnable()
{
}

void MapToolObj::OnDisable()
{
}

void MapToolObj::RenderForwardAlphaBlendImplementation(const ImplementationInfo& _ImplInfo)
{

}

void MapToolObj::RenderReady()
{
}
