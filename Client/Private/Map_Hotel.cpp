#include "stdafx.h"
#include "Map_Hotel.h"

#include "GameInstance.h"

CMap_Hotel::CMap_Hotel(ID3D11)
	:CGameObject{_pDevice, _pContext}
{
}

CMap_Hotel::CMap_Hotel(const CMap_Hotel& Prototype)
	:CGameObject(Prototype)
{
}

HRESULT CMap_Hotel::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMap_Hotel::Initialize(void* _pArg)
{
	if (FAILED(__super::Initialize(_pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;


	return S_OK;
}

void CMap_Hotel::Priority_Update(_float _fTimeDelta){}

void CMap_Hotel::Update(_float _fTimeDelta){}

void CMap_Hotel::Late_Update(_float _fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CMap_Hotel::Render()
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
		if (i == 11 || i == 14 || i == 18 || i == 19 || i == 24 || i == 25 || i == 32 || i == 40)
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

HRESULT CMap_Hotel::Ready_Components()
{
	//For Com_Shader
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxModel"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	//For Com_Model
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Hotel"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;


	return S_OK;
}

CMap_Hotel* CMap_Hotel::Create(ID3D11)
{
	CMap_Hotel* pInstance = new CMap_Hotel(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CMap_Hotel"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMap_Hotel::Clone(void* _pArg)
{
	CMap_Hotel* pInstance = new CMap_Hotel(*this);

	if (FAILED(pInstance->Initialize(_pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CMap_Hotel"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMap_Hotel::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
}
