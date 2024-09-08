#pragma once

#include "Client_Defines.h"
#include "UIObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CCollectibleUI :
    public CUIObject
{
private:
    CCollectibleUI(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
    CCollectibleUI(const CCollectibleUI& Prototype);
    virtual ~CCollectibleUI() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual void Set_Proj(_float _fX, _float _fY);

private:
	class CShader*			m_pShaderCom = { nullptr };
	class CTexture*			m_pTextureCom = { nullptr };
	class CVIBuffer_Rect*	m_pVIBufferCom = { nullptr };
	COLLECTIBLEID m_eCollectibleID = { SAVECOLLECTIBLE_END };

private:
	HRESULT Ready_Components();
	void Set_Orthogonal();

public:
	static CCollectibleUI* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END