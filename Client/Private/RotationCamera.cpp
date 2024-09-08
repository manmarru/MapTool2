#include "stdafx.h"

#include "RotationCamera.h"

#include "GameInstance.h"

CRotationCamera::CRotationCamera(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
    :CCamera{_pDevice, _pContext}
{
}

CRotationCamera::CRotationCamera(const CRotationCamera& Prototype)
    :CCamera(Prototype)
{
}

HRESULT CRotationCamera::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CRotationCamera::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;



    return S_OK;
}

void CRotationCamera::Priority_Update(_float fTimeDelta)
{

}

void CRotationCamera::Update(_float fTimeDelta)
{
}

void CRotationCamera::Late_Update(_float fTimeDelta)
{

}

HRESULT CRotationCamera::Render()
{
    return S_OK;
}

void CRotationCamera::Enter_Camera()
{
	m_vTargetPos = m_pGameInstance->Get_Target();
}

void CRotationCamera::Camera_Frame(_float _fTimeDelta)
{
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat3(&m_vTargetPos) + 3.f * _vector{ sin(XMConvertToRadians(3.6f * m_fWeight)) ,2.f, cos(XMConvertToRadians(3.6f * m_fWeight)) });
	m_pTransformCom->LookAt(XMLoadFloat3(&m_vTargetPos));
	__super::Priority_Update(_fTimeDelta);

	//2차 매개변수방정식 [0, 5초] x^2 - 5x
	m_fWeight += 20 * _fTimeDelta;
	if (m_fWeight >= 100)
	{
		m_fWeight = 0;
		m_pGameInstance->Set_CameraIndex(1);
	}

}

CRotationCamera* CRotationCamera::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CRotationCamera* pInstance = new CRotationCamera(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CRotationCamera"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CRotationCamera::Clone(void* pArg)
{
	CRotationCamera* pInstance = new CRotationCamera(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CRotationCamera"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRotationCamera::Free()
{
	__super::Free();
}
