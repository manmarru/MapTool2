#pragma once

#include "Base.h"

/* 1. ��ü���� �׸��� ������ ���� �����ϰ�. ������ ������� ��ü���� �����Լ��� ȣ���Ѵ�.  */
/* 2. 3D��� �ϴ��� ��ü���� �׸��� ������ ����� �߿��ϴ�. */
/* 3. �ϴ�, ����, UI���õ� ��ü���� �׸��� ������ ������ �ʼ������� �ʿ��ϴ�. */

BEGIN(Engine)

class CRenderer final : public CBase
{
public:
	enum RENDERGROUP { RG_PRIORITY, RG_NONBLEND, RG_PICKING, RG_BLEND, RG_UI, RG_END };
private:
	CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CRenderer() = default;

public:
	HRESULT Initialize();
	HRESULT Add_RenderObject(RENDERGROUP eRenderGroupID, class CGameObject* pRenderObject);
	HRESULT Draw();

private:
	ID3D11Device*				m_pDevice = { nullptr };
	ID3D11DeviceContext*		m_pContext = { nullptr };
	list<class CGameObject*>	m_RenderObjects[RG_END];
	class CGameInstance*		m_pGameInstance = { nullptr };

private:
	class CShader* m_pShader = { nullptr };
	class CVIBuffer_Rect* m_pVIBuffer = { nullptr };

	_float4x4				m_WorldMatrix{}, m_ViewMatrix{}, m_ProjMatrix{};

private:
	HRESULT Render_Priority();
	HRESULT Render_NonBlend();
	HRESULT Render_Picking();
	HRESULT Render_Blend();
	HRESULT Render_UI();

#ifdef _DEBUG
private:
	HRESULT Render_Debug();

#endif

public:
	static CRenderer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END