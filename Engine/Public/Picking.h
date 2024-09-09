#pragma once

#include "Base.h"

BEGIN(Engine)

class CPicking final : public CBase
{
private:
	CPicking(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual ~CPicking() = default;

public:
	HRESULT Initialize(HWND hWnd, _uint iWinSizeX, _uint iWinSizeY);
	void Update(); /* 윈도우좌표상(뷰포트상)의 마우스 좌표를 구하고 이 마우스를 좌표를 공용 스페이스(월드 스페이스)까지 내릴 수 있을 만큼 내린다. */
	void Transform_ToLocalSpace(const _float4x4& _WorldMatrix);
	_bool isPicked_InWorldSpace(const _float3& vPointA, const _float3& vPointB, const _float3& vPointC, _float3* pOut);
	_bool isPicked_InLocalSpace(const _float3& vPointA, const _float3& vPointB, const _float3& vPointC, _float3* pOut);
	POINT Get_MousePos() { return m_tMousePos; }

private:
	ID3D11Device*				m_pDevice = {nullptr};
	ID3D11DeviceContext*		m_pContext = {nullptr};
	HWND						m_hWnd = {};
	_uint						m_iWinSizeX = {};
	_uint						m_iWinSizeY = {};
	class CGameInstance*		m_pGameInstance = { nullptr };


private:
	_float3						m_vRayPos = {};
	_float3						m_vRayDir = {};
	POINT						m_tMousePos = {};

private:
	_float3						m_vRayPos_InLocalSpace = {};
	_float3						m_vRayDir_InLocalSpace = {};

public:
	static CPicking* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext, HWND hWnd, _uint iWinSizeX, _uint iWinSizeY);
	virtual void Free();
};

END