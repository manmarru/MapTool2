#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CModel;
class CShader;
END


BEGIN(Client)

class CItemBox_Beach :
    public CGameObject
{
public:
    typedef struct ITEMBOX_DESC : GAMEOBJECT_DESC
    {
        _wstring ModelTag;
    };
private:
    CItemBox_Beach(ID3D11);
    CItemBox_Beach(const CItemBox_Beach& Prototype);
    virtual ~CItemBox_Beach() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* _pArg);
    virtual void    Priority_Update(_float _fTimeDelta);
    virtual void    Update(_float _fTimeDelta);
    virtual void    Late_Update(_float _fTimeDelta);
    virtual HRESULT Render();
    virtual HRESULT Render_Picking() override;

public:
    _bool Item_Input(ITEMID _eItemID);
    void Remove_Item(_uint _iIndex);
    list<ITEMID>* Get_Inventory() { return &m_listInventory; 
    }


private:
    list<ITEMID> m_listInventory;

private:
    HRESULT Ready_Components(_wstring _ModelTag);

private:
    CModel* m_pModelCom = { nullptr };
    CShader* m_pShaderCom = { nullptr };

public:
    static CItemBox_Beach*        Create(ID3D11);
    virtual CGameObject*    Clone(void* _pArg);
    virtual void            Free() override;
};

END