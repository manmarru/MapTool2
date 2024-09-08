#pragma once
#include "stdafx.h"
#include "CCollectibleUI.h"
#include "GameInstance.h"

CCollectibleUI::CCollectibleUI(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	:CUIObject{ _pDevice, _pContext }
{
}

CCollectibleUI::CCollectibleUI(const CCollectibleUI& Prototype)
	:CUIObject(Prototype)
{
}

HRESULT CCollectibleUI::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCollectibleUI::Initialize(void* pArg)
{

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CCollectibleUI::Priority_Update(_float fTimeDelta)
{
}

void CCollectibleUI::Update(_float fTimeDelta)
{
}

void CCollectibleUI::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);

	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CCollectibleUI::Render()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pTextureCom->Bind_ShadeResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;
	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

void CCollectibleUI::Set_Proj(_float _fX, _float _fY)
{
	m_fX = _fX;
	m_fY = _fY;
}

HRESULT CCollectibleUI::Ready_Components()
{
	//com shader
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPosTex"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	//com Texture
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_CollectibleUI_Default"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* FOR.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	return S_OK;
}

void CCollectibleUI::Set_Orthogonal()
{

	_uint		iNumView = { 1 };
	D3D11_VIEWPORT		ViewportDesc{};

	m_pContext->RSGetViewports(&iNumView, &ViewportDesc);

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(ViewportDesc.Width, ViewportDesc.Height, 0.f, 1.f));

	m_fViewWidth = ViewportDesc.Width;
	m_fViewHeight = ViewportDesc.Height;

}

CCollectibleUI* CCollectibleUI::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
{
	CCollectibleUI* pInstance = new CCollectibleUI(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CCollectibleUI"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCollectibleUI::Clone(void* pArg)
{
	CCollectibleUI* pInstance = new CCollectibleUI(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CCollectibleUI"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCollectibleUI::Free()
{
	__super::Free();
}
