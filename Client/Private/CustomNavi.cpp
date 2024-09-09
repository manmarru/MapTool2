#include "stdafx.h"
#include "CustomNavi.h"

#include "GameInstance.h"

CCustomNavi::CCustomNavi(ID3D11)
	:CGameObject{_pDevice, _pContext}
{
}

CCustomNavi::CCustomNavi(const CCustomNavi& Prototype)
	:CGameObject(Prototype)
{
}

HRESULT CCustomNavi::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CCustomNavi::Initialize(void* _pArg)
{
	if (FAILED(__super::Initialize(_pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CCustomNavi::Priority_Update(_float _fTimeDelta)
{
}

void CCustomNavi::Update(_float _fTimeDelta)
{
}

void CCustomNavi::Late_Update(_float _fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CCustomNavi::Render()
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
		m_pModelCom->Bind_MeshBoneMatrices(m_pShaderCom, "g_BoneMatrices", i);

		if (FAILED(m_pShaderCom->Begin(0)))
			return E_FAIL;
		if (FAILED(m_pModelCom->Render(i)))
			return E_FAIL;

	}

	return S_OK;
}

HRESULT CCustomNavi::Ready_Components()
{
	//네비메쉬
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Navigation"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	m_pShaderCom = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxCustomNavi.hlsl"), VTXPOS::Elements, VTXPOS::iNumElements);
	if (nullptr == m_pShaderCom)
		return E_FAIL;


	return S_OK;
}

CCustomNavi* CCustomNavi::Create(ID3D11)
{
	CCustomNavi* pInstance = new CCustomNavi(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CCustomNavi"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCustomNavi::Clone(void* _pArg)
{
	CCustomNavi* pInstance = new CCustomNavi(*this);
	if (FAILED(pInstance->Initialize(_pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CCustomNavi"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCustomNavi::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}
