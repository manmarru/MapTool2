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

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    m_eType = Desc->eType;
    Set_Rotation(Desc->vRotation.x, Desc->vRotation.y, Desc->vRotation.z);

    return S_OK;
}

void CSaveObj::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
}

void CSaveObj::Set_Rotation(_float _RotationX, _float _RotationY, _float RotationZ)
{
    m_vRotation = _float3{ _RotationX, _RotationY, RotationZ };
    m_pTransformCom->Rotation(_RotationX, _RotationY, RotationZ);
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
