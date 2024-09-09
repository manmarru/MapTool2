#include "CameraManager.h"

#include "Camera.h"

CCameraManager::CCameraManager()
{
}

HRESULT CCameraManager::Initialize()
{
    m_mapCamera.clear();

    return S_OK;
}

void CCameraManager::Protrity_Update(_float fTimeDelta)
{
    if (nullptr == m_mapCamera[m_iCurIndex])
        return;
    m_mapCamera[m_iCurIndex]->Camera_Frame(fTimeDelta);
}

void CCameraManager::Update()
{
}

void CCameraManager::Late_Update()
{
}

HRESULT CCameraManager::Insert_Camera(_int _iCameraID, CGameObject* _pCamera)
{
    if (m_mapCamera.end() == m_mapCamera.find(_iCameraID))
    {
        m_mapCamera.emplace(_iCameraID, (CCamera*)_pCamera);

        Safe_AddRef(_pCamera);

        return S_OK;
    }
    MSG_BOX(TEXT("Camera_Not_Exist"));
    return E_FAIL;
}

void CCameraManager::Set_CameraIndex(_int _iCameraID)
{
    m_iCurIndex = _iCameraID;
    m_mapCamera[_iCameraID]->Enter_Camera();
}

CCameraManager* CCameraManager::Create()
{
    CCameraManager* pInstance = new CCameraManager;
    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX(TEXT("Failed_to_Create : CCamera_Manager"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CCameraManager::Free()
{
    Safe_Release(m_mapCamera[1]);
    Safe_Release(m_mapCamera[2]);

    m_mapCamera.clear();
}
