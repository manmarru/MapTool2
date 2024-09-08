#pragma once

#include "Base.h"
#include "GameObject.h"

BEGIN(Engine)
class CCamera;
class CCameraManager final :
    public CBase
{
private:
    CCameraManager();
    virtual ~CCameraManager() = default;

public:
    HRESULT Initialize();
    void Protrity_Update(_float fTimeDelta);
    void Update();
    void Late_Update();

public:
    HRESULT Insert_Camera(_int _iCameraID, CGameObject* _pCamera);
    void Set_CameraIndex(_int _iCameraID);
    void Set_Target(_float3 _vPos) { m_vTargetPos = _vPos; }
    _float3 Get_Target() { return m_vTargetPos; }
private:
    map<_int, CCamera*> m_mapCamera;
    _int m_iCurIndex{ 0 }; // 현재 사용하고있는 카메라 인덱스
    _float3 m_vTargetPos = {0.f, 0.f, 0.f};


public:
    static CCameraManager* Create();
    virtual void Free() override;
};

END