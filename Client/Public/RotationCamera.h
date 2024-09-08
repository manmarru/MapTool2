#pragma once

#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Client)

class CRotationCamera final :
    public CCamera
{
private:
    CRotationCamera(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
    CRotationCamera(const CRotationCamera& Prototype);
    virtual ~CRotationCamera() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual void Enter_Camera() override;
	virtual void Camera_Frame(_float _fTimeDelta) override;

private:
	_vector m_vStart;
	_vector m_vGoal;

	_float m_fSpeed = { 1.f };
	_float m_fWeight = { 0.f };
	_float3 m_vTargetPos{ 0.f, 0.f, 0.f };

public:
	static CRotationCamera* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

	
};

END