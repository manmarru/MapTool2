#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CModel;
END

class CMap_Forest :
    public CGameObject
{
private:
    CMap_Forest(ID3D11);
    CMap_Forest(const CMap_Forest& Prototype);
    virtual ~CMap_Forest() = default;

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
    static CMap_Forest* Create(ID3D11);
    virtual CGameObject* Clone(void* _pArg);
    virtual void Free() override;
};

