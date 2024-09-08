#include "stdafx.h"

#include "SaveObj.h"

#include "GameInstance.h"

CSaveObj::CSaveObj(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
    :CGameObject{_pDevice, _pContext}
{
}

CSaveObj::CSaveObj(const CSaveObj& Prototype)
    :CGameObject(Prototype)
    , m_eType{Prototype.m_eType}
{
}

HRESULT CSaveObj::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CSaveObj::Initialize(void* pArg)
{
    SAVEOBJ_DESC* Desc = (SAVEOBJ_DESC*)pArg;

    m_eType = Desc->eType;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    return S_OK;
}

void CSaveObj::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
}

void CSaveObj::Set_RotateX(_float _fTheta)
{
    m_pTransformCom->Rotation(_fvector{ 1.f, 0.f, 0.f }, XMConvertToRadians(_fTheta));
}

void CSaveObj::Set_RotateY(_float _fTheta)
{
    m_pTransformCom->Rotation(_fvector{ 0.f, 1.f, 0.f }, XMConvertToRadians(_fTheta));
}

void CSaveObj::Set_RotateZ(_float _fTheta)
{
    m_pTransformCom->Rotation(_fvector{ 0.f, 0.f, 1.f }, XMConvertToRadians(_fTheta));
}

void CSaveObj::Set_Pos(_float3 _vPos)
{
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat3(&_vPos));
}

void CSaveObj::Set_PosAll(_float _fPos)
{
}

void CSaveObj::Set_Scale(_float3 _vScale)
{
}

void CSaveObj::Plus_Pos(_float3 _fInput)
{
    m_pTransformCom->Plus_Pos(_fInput);
}

_float3 CSaveObj::Get_Scale()
{
    return m_pTransformCom->Get_Scaled();
}

_float3 CSaveObj::Get_Pos()
{
    _float3 temp;
    XMStoreFloat3(&temp, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
    return temp;
}


void CSaveObj::Free()
{
    __super::Free();
}
