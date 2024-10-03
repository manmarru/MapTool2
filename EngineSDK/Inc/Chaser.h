#pragma once

#include "Base.h"

BEGIN(Engine)

class CChaser final :
    public CBase
{
private:
    CChaser();
    virtual ~CChaser() = default;

public:
    void Set_ChaserTargetPos(_float3 _vPos) { m_TargetPos = _vPos; }
    _float3 Get_ChaserTargetPos() { return m_TargetPos; }
    _uint Invest() { return m_investing++; }


private:
    _float3 m_TargetPos = {};
    _uint m_investing = { 1 }; // 부여해줄 번호 ( 부여하면서 +1), 0은 사용하지 않을거임

public:
    static CChaser* Create() { return new CChaser(); }
    virtual void Free();
};

END