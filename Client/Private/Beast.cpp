#include "stdafx.h"

#include "Beast.h"

#include "GameInstance.h"

CBeast::CBeast(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
    :CSaveObj{_pDevice, _pContext}
{
}

CBeast::CBeast(const CBeast& Prototype)
    :CSaveObj(Prototype)
{
}

HRESULT CBeast::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CBeast::Initialize(void* pArg)
{
	SAVEOBJ_DESC* Desc = static_cast<SAVEOBJ_DESC*>(pArg);
	m_eMonsterID = (SAVEMONSTERID)Desc->eMonsterID;
    if (FAILED(__super::Initialize(Desc)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

	m_vecInventory.reserve(8);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat3(&Desc->vPos));

	m_pModelCom->SetUp_Animation(0, true);

    return S_OK;
}

void CBeast::Priority_Update(_float fTimeDelta)
{
}

void CBeast::Update(_float fTimeDelta)
{
	//m_pModelCom->Play_Animation(fTimeDelta);
}

void CBeast::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);

    m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CBeast::Render()
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



void CBeast::Pop_Item(int iNum)
{
	auto iter = m_vecInventory.begin();
	iter + iNum;
	m_vecInventory.erase(iter);
}



HRESULT CBeast::Ready_Components()
{
	/* FOR.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxModel"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	Load_Model();

	return S_OK;
}

HRESULT CBeast::Load_Model()
{
	/* FOR.Com_Model */
	switch (m_eMonsterID)
	{
	case Client::SAVEMONSTER_BAT:
		if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Bat"),
			TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
			return E_FAIL;
		break;
	case Client::SAVEMONSTER_BEAR:
		if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Bear"),
			TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
			return E_FAIL;
		break;
	case Client::SAVEMONSTER_BOAR:
		if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Boar"),
			TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
			return E_FAIL;
		break;
	case Client::SAVEMONTSER_CHICKEN:
		if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Chicken"),
			TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
			return E_FAIL;
		break;
	case Client::SAVEMONSTER_DOG:
		if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Dog"),
			TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
			return E_FAIL;
		break;
	case Client::SAVEMONSTER_WOLF:
		if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Wolf"),
			TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
			return E_FAIL;
		break;
	default:
		return E_FAIL;
	}

	return S_OK;
}



CBeast* CBeast::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CBeast* pInstance = new CBeast(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CBeast"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBeast::Clone(void* pArg)
{
	CBeast* pInstance = new CBeast(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CBeast"));
		Safe_Release(pInstance);
	}
	
	return pInstance;
}

void CBeast::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);

	Safe_Release(m_pModelCom);
}
