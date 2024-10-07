#include "stdafx.h"

#include "ItemBox.h"

#include "GameInstance.h"

CItemBox::CItemBox(ID3D11)
    :CGameObject{ _pDevice, _pContext }
{
}

CItemBox::CItemBox(const CItemBox& Prototype)
    :CGameObject(Prototype)
{
}

HRESULT CItemBox::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CItemBox::Initialize(void* _pArg)
{
    ITEMBOX_DESC* pDesc = static_cast<ITEMBOX_DESC*>(_pArg);

    if (__super::Initialize(pDesc))
        return E_FAIL;

    if (FAILED(Ready_Components(pDesc->ModelTag)))
        return E_FAIL;

    m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(-165.877, 0.f, -91.058f, 1.f));

    return S_OK;
}

void CItemBox::Priority_Update(_float _fTimeDelta)
{
    __super::Priority_Update(_fTimeDelta); // none
}

void CItemBox::Update(_float _fTimeDelta)
{
    __super::Update(_fTimeDelta); // none
}

void CItemBox::Late_Update(_float _fTimeDelta)
{
    __super::Late_Update(_fTimeDelta);//none

    m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
    m_pGameInstance->Add_RenderObject(CRenderer::RG_PICKING, this);
}

HRESULT CItemBox::Render()
{
    if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;

    _uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

    for (size_t i = 0; i < iNumMeshes; i++)
    {
        if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", aiTextureType_DIFFUSE, i)))
            return E_FAIL;

        if (FAILED(m_pShaderCom->Begin(0)))
            return E_FAIL;

        if (FAILED(m_pModelCom->Render(i)))
            return E_FAIL;
    }

    return S_OK;
}

HRESULT CItemBox::Render_Picking()
{
    if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;

    _uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

    for (size_t i = 0; i < iNumMeshes; i++)
    {
        if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", aiTextureType_DIFFUSE, i)))
            return E_FAIL;

        if (FAILED(m_pShaderCom->Begin(1)))
            return E_FAIL;

        if (FAILED(m_pModelCom->Render(i)))
            return E_FAIL;
    }


    return S_OK;
}

HRESULT CItemBox::Ready_Components(_wstring _ModelTag)
{
    //For Com_Shader
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxModel"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    //For Com_Model
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, _ModelTag,
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;

    return S_OK;
}

CItemBox* CItemBox::Create(ID3D11)
{
    CItemBox* pInstance = new CItemBox(_pDevice, _pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed To Created : CItemBox"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CItemBox::Clone(void* _pArg)
{
    CItemBox* pInstance = new CItemBox(*this);

    if (FAILED(pInstance->Initialize(_pArg)))
    {
        MSG_BOX(TEXT("Failed To Cloned : CItemBox"));
        Safe_Release(pInstance);
    }

    m_iObjNum = m_pGameInstance->Invest();

    return pInstance;
}

void CItemBox::Free()
{
    __super::Free();

    Safe_Release(m_pModelCom);
    Safe_Release(m_pShaderCom);
}
