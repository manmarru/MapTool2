#pragma once

#include "Client_Defines.h"
#include "SaveObj.h"

BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)

class CCollectibleUI;
class CCollectible :
	public CSaveObj
{
private:
	CCollectible(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CCollectible(const CCollectible& Prototype);
	virtual ~CCollectible() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	class CShader* m_pShaderCom = { nullptr };
	class CModel* m_pModelCom = { nullptr };

	virtual _int Get_ID() override { return (_int)m_eCollectibleID; }

private:
	CCollectibleUI* m_pUI{ nullptr };
	COLLECTIBLEID m_eCollectibleID = { SAVECOLLECTIBLE_END };

private:
	HRESULT Ready_Component();
	HRESULT Load_Model();

public:
	static CCollectible* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END