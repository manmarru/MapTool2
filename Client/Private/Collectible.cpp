#pragma once
#include "stdafx.h"

#include "Collectible.h"

#include "CCollectibleUI.h"

#include "GameInstance.h"

CCollectible::CCollectible(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	:CSaveObj{_pDevice, _pContext}
{
}

CCollectible::CCollectible(const CCollectible& Prototype)
	:CSaveObj(Prototype)
{
}

HRESULT CCollectible::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCollectible::Initialize(void* pArg)
{
	SAVEOBJ_DESC* Desc = static_cast<SAVEOBJ_DESC*>(pArg);
	m_eCollectibleID = (COLLECTIBLEID)Desc->eMonsterID;

	if (FAILED(__super::Initialize(Desc)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat3(&Desc->vPos));

	CUIObject::UI_DESC UIDesc = {};
	UIDesc.fSizeX = 100;
	UIDesc.fSizeY = 100;
	UIDesc.fX = 0;
	UIDesc.fY = 0;
	m_pUI = (CCollectibleUI*)m_pGameInstance->Add_CloneObject_ToLayer_Get(LEVEL_GAMEPLAY, TEXT("Layer_UI"), TEXT("Prototype_GameObject_CollectibleUI"), &UIDesc);

	return S_OK;
}

void CCollectible::Priority_Update(_float fTimeDelta)
{
}

void CCollectible::Update(_float fTimeDelta)
{

}

void CCollectible::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);

	_matrix proj = XMLoadFloat4x4(&m_pTransformCom->Get_WorldMatrix())
		* m_pGameInstance->Get_Transform_Matrix(CPipeLine::D3DTS_VIEW)
		* m_pGameInstance->Get_Transform_Matrix(CPipeLine::D3DTS_PROJ);

	m_pUI->Set_Proj(XMVectorGetX(proj.r[3]), XMVectorGetY(proj.r[3]));

	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CCollectible::Render()
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
		m_pModelCom->Bind_MeshBoneMatrices(m_pShaderCom, "g_BoneMatrices", i);

		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", aiTextureType_DIFFUSE, i)))
			return E_FAIL;
		/*if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_NormalTexture", aiTextureType_NORMALS, i)))
			return E_FAIL;*/

		if (FAILED(m_pShaderCom->Begin(0)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(i)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CCollectible::Ready_Component()
{
	/* FOR.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxModel"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	Load_Model();

	return S_OK;
}

HRESULT CCollectible::Load_Model()
{
	switch (m_eCollectibleID)
	{
	case Client::SAVECOLLECTIBLE_STONE:
		if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Stone"),
			TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
			return E_FAIL;
		break;
	case Client::SAVECOLLECTIBLE_FLOWER:
		if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Flower"),
			TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
			return E_FAIL;
		break;
	case Client::SAVECOLLECTIBLE_WATER:
		if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Water"),
			TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
			return E_FAIL;
		break;
	case Client::SAVECOLLECTIBLE_BRANCH:
		if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Branch"),
			TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
			return E_FAIL;
		break;
	default:
		return E_FAIL;
	}

	return S_OK;
}

CCollectible* CCollectible::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CCollectible* pInstance = new CCollectible(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CCollectible"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCollectible::Clone(void* pArg)
{
	CCollectible* pInstance = new CCollectible(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CCollectible"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCollectible::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);

	Safe_Release(m_pModelCom);

}
