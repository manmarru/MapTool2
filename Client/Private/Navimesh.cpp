#include "stdafx.h"
#include "Navimesh.h"

#include "GameInstance.h"

CNavimesh::CNavimesh(ID3D11)
	:CGameObject{ _pDevice, _pContext }
{
}

CNavimesh::CNavimesh(const CNavimesh& Prototype)
	:CGameObject(Prototype)
{
}

HRESULT CNavimesh::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CNavimesh::Initialize(void* _pArg)
{
	if (FAILED(__super::Initialize(_pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CNavimesh::Priority_Update(_float _fTimeDelta) {}

void CNavimesh::Update(_float _fTimeDelta) {}

void CNavimesh::Late_Update(_float _fTimeDelta) {}

HRESULT CNavimesh::Render()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Inverse_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Inverse_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	_uint iNumMeshes = m_pModelCom->Get_NumMeshes();

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

_bool CNavimesh::isPicking(_float3* pOut)
{
	return true;
}

HRESULT CNavimesh::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxModel"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Navimesh"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

CNavimesh* CNavimesh::Create(ID3D11)
{
	CNavimesh* pInstance = new CNavimesh(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CNavimesh"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CNavimesh::Clone(void* _pArg)
{
	CNavimesh* pInstance = new CNavimesh(*this);

	if (FAILED(pInstance->Initialize(_pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CNavimesh"));
		Safe_Release(pInstance);
	}

	return nullptr;
}

void CNavimesh::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
}
