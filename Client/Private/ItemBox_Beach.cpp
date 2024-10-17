#include "stdafx.h"

#include "ItemBox_Beach.h"

#include "GameInstance.h"

CItemBox_Beach::CItemBox_Beach(ID3D11)
    :CGameObject{ _pDevice, _pContext }
{
}

CItemBox_Beach::CItemBox_Beach(const CItemBox_Beach& Prototype)
    :CGameObject(Prototype)
{
}

HRESULT CItemBox_Beach::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CItemBox_Beach::Initialize(void* _pArg)
{
    ITEMBOX_DESC* pDesc = static_cast<ITEMBOX_DESC*>(_pArg);

    if (__super::Initialize(pDesc))
        return E_FAIL;

    if (FAILED(Ready_Components(pDesc->ModelTag)))
        return E_FAIL;

    m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(-22.923f, 0.f, -101.644, 1.f));

    m_iObjNum = m_pGameInstance->Invest();

    m_pGameInstance->Input_Sygnature(this);

    return S_OK;
}

void CItemBox_Beach::Priority_Update(_float _fTimeDelta)
{
    __super::Priority_Update(_fTimeDelta); // none
}

void CItemBox_Beach::Update(_float _fTimeDelta)
{
    __super::Update(_fTimeDelta); // none
}

void CItemBox_Beach::Late_Update(_float _fTimeDelta)
{
    __super::Late_Update(_fTimeDelta);//none

    m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
    m_pGameInstance->Add_RenderObject(CRenderer::RG_PICKING, this);
}

HRESULT CItemBox_Beach::Render()
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

HRESULT CItemBox_Beach::Render_Picking()
{
    if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_iObjNum", &m_iObjNum, sizeof(_uint))))
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

_bool CItemBox_Beach::Item_Input(ITEMID _eItemID)
{
    if (6 <= m_listInventory.size())
        return false;

    m_listInventory.push_back(_eItemID);

    return true;
}

void CItemBox_Beach::Remove_Item(_uint _iIndex)
{
    auto iter = m_listInventory.begin();
    for (size_t i = 0; i < _iIndex; i++)
    {
        ++iter;
    }
    m_listInventory.erase(iter);
}

HRESULT CItemBox_Beach::Ready_Components(_wstring _ModelTag)
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

CItemBox_Beach* CItemBox_Beach::Create(ID3D11)
{
    CItemBox_Beach* pInstance = new CItemBox_Beach(_pDevice, _pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed To Created : CItemBox_Beach"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CItemBox_Beach::Clone(void* _pArg)
{
    CItemBox_Beach* pInstance = new CItemBox_Beach(*this);

    if (FAILED(pInstance->Initialize(_pArg)))
    {
        MSG_BOX(TEXT("Failed To Cloned : CItemBox_Beach"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CItemBox_Beach::Free()
{
    __super::Free();

    Safe_Release(m_pModelCom);
    Safe_Release(m_pShaderCom);
}
