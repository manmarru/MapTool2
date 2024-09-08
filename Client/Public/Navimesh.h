#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)
class CNavimesh :
    public CGameObject
{
private:
    CNavimesh(ID3D11);
    CNavimesh(const CNavimesh& Prototype);
    virtual ~CNavimesh() = default;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(void* _pArg) override;
    void Priority_Update(_float _fTimeDelta) override;
    void Update(_float _fTimeDelta) override;
    void Late_Update(_float _fTimeDelta) override;
    HRESULT Render() override;

public:
    _bool isPicking(_float3* pOut);
    _float4x4 Get_WorldMatrix() { return m_pTransformCom->Get_WorldMatrix(); }



private:
    HRESULT  Ready_Components();

private:
    CModel* m_pModelCom;
    CShader* m_pShaderCom;

public:
    static CNavimesh* Create(ID3D11);
    virtual CGameObject* Clone(void* _pArg);
    virtual void Free() override;
};

END