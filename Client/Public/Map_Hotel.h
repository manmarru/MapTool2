#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CModel;
END

class CMap_Hotel :
    public CGameObject
{
private:
    CMap_Hotel(ID3D11);
    CMap_Hotel(const CMap_Hotel& Prototype);
    virtual ~CMap_Hotel() = default;

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* _pArg) override;
    virtual void Priority_Update(_float _fTimeDelta) override;
    virtual void Update(_float _fTimeDelta) override;
    virtual void Late_Update(_float _fTimeDelta) override;
    virtual HRESULT Render() override;

private:
    CShader*    m_pShaderCom = { nullptr };
    CModel*     m_pModelCom = { nullptr };

private:
    HRESULT Ready_Components();

public:
    static CMap_Hotel* Create(ID3D11);
    virtual CGameObject* Clone(void* _pArg);
    virtual void Free() override;
};

