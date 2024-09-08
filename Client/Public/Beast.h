#pragma once

#include "Client_Defines.h"
#include "SaveObj.h"

BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)

class CBeast :
    public CSaveObj
{
private:
    CBeast(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
    CBeast(const CBeast& Prototype);
    virtual ~CBeast() = default;

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


	virtual _int Get_ID() override { return (_int)m_eMonsterID; }

	vector<ITEMID>* Get_Inventory() { return &m_vecInventory; }

public:
	void Add_Item(ITEMID _eItemID) { m_vecInventory.push_back(_eItemID); }
	void Pop_Item(int iNum);

private:
	SAVEMONSTERID m_eMonsterID = { SAVEMONSTER_END };
	vector<ITEMID> m_vecInventory;


private:
	HRESULT Ready_Components();
	HRESULT Load_Model();

public:
	static CBeast* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END