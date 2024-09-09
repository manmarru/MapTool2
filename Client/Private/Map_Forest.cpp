#include "stdafx.h"
#include "Map_Forest.h"

#include "GameInstance.h"

CMap_Forest::CMap_Forest(ID3D11)
	:CGameObject{_pDevice, _pContext}
{
}

CMap_Forest::CMap_Forest(const CMap_Forest& Prototype)
	:CGameObject(Prototype)
{
}

HRESULT CMap_Forest::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMap_Forest::Initialize(void* _pArg)
{
	if (FAILED(__super::Initialize(_pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;


	return S_OK;
}

void CMap_Forest::Priority_Update(_float _fTimeDelta){}

void CMap_Forest::Update(_float _fTimeDelta){}

void CMap_Forest::Late_Update(_float _fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CMap_Forest::Render()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	_uint iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (size_t i = 0; i < iNumMeshes; i++)
	{
		if (i == 13 || i == 23 || i == 14 || i == 24)
			continue;

		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", aiTextureType_DIFFUSE, i)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Bind_MeshBoneMatrices(m_pShaderCom, "g_BoneMatrices", i)))
			return E_FAIL;

		if (FAILED(m_pShaderCom->Begin(0)))
			return E_FAIL;
		if (FAILED(m_pModelCom->Render(i)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CMap_Forest::Ready_Components()
{
	//For Com_Shader
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxModel"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	//For Com_Model
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Forest"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;


	return S_OK;
}

CMap_Forest* CMap_Forest::Create(ID3D11)
{
	CMap_Forest* pInstance = new CMap_Forest(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CMap_Forest"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMap_Forest::Clone(void* _pArg)
{
	CMap_Forest* pInstance = new CMap_Forest(*this);

	if (FAILED(pInstance->Initialize(_pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CMap_Forest"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMap_Forest::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
}
