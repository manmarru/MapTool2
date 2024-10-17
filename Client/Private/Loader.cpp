#include "stdafx.h"
#include "..\Public\Loader.h"

#include "Terrain.h"
#include "CustomNavi.h"
#include "Map_Beach.h"
#include "Map_Forest.h"
#include "Map_Hotel.h"
#include "Navigation.h"

#include "FreeCamera.h"
#include "RotationCamera.h"

#include "BackGround.h"

#include "Monster.h"
#include "Beast.h"
#include "ItemBox.h"
#include "ItemBox_Beach.h"
#include "Collectible.h"
#include "CCollectibleUI.h"


#include "GameInstance.h"


CLoader::CLoader(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: m_pDevice { pDevice }
	, m_pContext { pContext }
	, m_pGameInstance { CGameInstance::Get_Instance()}
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}


_uint APIENTRY LoadingMain(void* pArg)
{
	CoInitializeEx(nullptr, 0);

	CLoader*		pLoader = static_cast<CLoader*>(pArg);

	if(FAILED(pLoader->Loading()))
		return 1;
		
	return 0;
}


HRESULT CLoader::Initialize(LEVELID eNextLevelID)
{
	m_eNextLevelID = eNextLevelID;

	InitializeCriticalSection(&m_CriticalSection);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, LoadingMain, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;


	return S_OK;
}

HRESULT CLoader::Loading()
{
	EnterCriticalSection(&m_CriticalSection);

	HRESULT			hr = {};

	switch (m_eNextLevelID)
	{
	case LEVEL_LOGO:
		hr = Ready_Resources_For_LogoLevel();
		break;
	case LEVEL_GAMEPLAY:
		hr = Ready_Resources_For_GamePlayLevel();
		break;
	}

	LeaveCriticalSection(&m_CriticalSection);

	return hr;
}

void CLoader::Draw_LoadingText()
{
	SetWindowText(g_hWnd, m_szLoadingText);
}

HRESULT CLoader::Ready_Resources_For_LogoLevel()
{
	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩중입니다."));
	/* For. Prototype_Component_Texture_BackGround */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_BackGround"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Logo/Logo.png"), 1))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("모델을(를) 로딩중입니다."));


	lstrcpy(m_szLoadingText, TEXT("사운드을(를) 로딩중입니다."));


	lstrcpy(m_szLoadingText, TEXT("객체원형을(를) 로딩중입니다."));
	/* For. Prototype_GameObject_BackGround */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BackGround"),
		CBackGround::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Ready_Resources_For_GamePlayLevel()
{
	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩중입니다."));
	/* For. Prototype_Component_Texture_Terrain*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Terrain"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Grass_%d.dds"), 2))))
		return E_FAIL;

	/* For. Prototype_Component_Texture_CollectibleUI_Default*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_CollectibleUI_Default"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/UI/Collectible/Default.png"), 1))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("모델을(를) 로딩중입니다."));
	/* For. Prototype_Component_VIBuffer_Terrain*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pDevice, m_pContext, 70., 70))))
		return E_FAIL;

	_matrix		PreTransformMatrix = XMMatrixIdentity();

	///* For. Prototype_Component_Model_Fiona*/
	//PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f));
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Fiona"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_ANIM, "../Bin/Resources/Models/Fiona/Fiona.fbx", PreTransformMatrix))))
	//	return E_FAIL;

	//네비메쉬 모델 출력용
	////For Prototype_Component_Model_Navigation
	//PreTransformMatrix = XMMatrixIdentity();
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Navigation"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/map/NaviMesh/NaviMesh.fbx", PreTransformMatrix))))
	//	return E_FAIL;

	////For Prototype_Component_Model_Beach
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Beach"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/map/Beach/Rocks_SandBeach.fbx"))))
	//	return E_FAIL;

	////For Prototype_Component_Model_Forest
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Forest"),
	//	CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/map/Forest/Ivy_Forest_Set.fbx"))))
	//	return E_FAIL;

	//For Prototype_Component_Model_Hotel
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Hotel"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/map/Hotel/Rocks_Hotel.fbx"))))
		return E_FAIL;

#pragma region 아이템박스
	wstring boxName;
	_char ModelPath_ItemBox[MAX_PATH];
	wchar_t buffer[MAX_PATH];
	PreTransformMatrix = XMMatrixIdentity();// *XMMatrixRotationY(XMConvertToRadians(-90.0f));
	for (size_t i = 1; i < 29 + 1; i++)
	{
		swprintf(buffer, MAX_PATH, TEXT("Prototype_Component_Model_ItemSpot_%d"), i);
		boxName = buffer;

		snprintf(ModelPath_ItemBox, MAX_PATH, "../Bin/Resources/Models/ItemBox/Hotel/ItemSpot_(%d)/ItemSpot.fbx", i);
		//For Prototype_Component_Model_ItemSpot_%d
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, boxName,
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, ModelPath_ItemBox, PreTransformMatrix))))
			return E_FAIL;
	}
	for (size_t i = 1; i < 28 + 1; i++)
	{
		swprintf(buffer, MAX_PATH, TEXT("Prototype_Component_Model_ItemSpot_%d"), i + 29);
		boxName = buffer;

		snprintf(ModelPath_ItemBox, MAX_PATH, "../Bin/Resources/Models/ItemBox/Beach/ItemSpot_(%d)/ItemSpot.fbx", i);
		//For Prototype_Component_Model_ItemSpot_%d
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, boxName,
			CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, ModelPath_ItemBox, PreTransformMatrix))))
			return E_FAIL;
	}


#pragma endregion


	ifstream LoadStream;
	LoadStream.open("../Bin/Data/Navigation.txt", ios::in | ios::binary | ios::app);

	lstrcpy(m_szLoadingText, TEXT("네비게이션을(를) 로딩중입니다."));
	/* For.Prototype_Component_Navigation */
	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Navigation"),
		CNavigation::Create(m_pDevice, m_pContext, &LoadStream, PreTransformMatrix))))
		return E_FAIL;

	LoadStream.close();



#pragma region 야생동물
	/* For. Prototype_Component_Model_Bat*/
	PreTransformMatrix = XMMatrixIdentity() * XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Bat"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Beast/Bat/Bat_01.fbx", PreTransformMatrix))))
		return E_FAIL;

	/* For. Prototype_Component_Model_Bear*/
	PreTransformMatrix = XMMatrixIdentity() * XMMatrixRotationY(XMConvertToRadians(180.0f)) * XMMatrixTranslation(4.f, 0.f, 0.f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Bear"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Beast/Bear/Bear_01.fbx", PreTransformMatrix))))
		return E_FAIL;

	/* For. Prototype_Component_Model_Boar*/
	PreTransformMatrix = XMMatrixIdentity() * XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Boar"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Beast/Boar/Boar_01.fbx", PreTransformMatrix))))
		return E_FAIL;

	/* For. Prototype_Component_Model_Chicken*/
	PreTransformMatrix = XMMatrixIdentity() * XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Chicken"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Beast/Chicken/Chicken_01.fbx", PreTransformMatrix))))
		return E_FAIL;

	/* For. Prototype_Component_Model_Dog*/
	PreTransformMatrix = XMMatrixIdentity() * XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Dog"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Beast/Dog/Dog_01.fbx", PreTransformMatrix))))
		return E_FAIL;

	/* For. Prototype_Component_Model_Wolf*/
	PreTransformMatrix = XMMatrixIdentity() * XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Wolf"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Beast/Wolf/Wolf_01.fbx", PreTransformMatrix))))
		return E_FAIL;
#pragma endregion

#pragma region 채집물
	/* For. Prototype_Component_Model_Branch*/
	PreTransformMatrix = XMMatrixIdentity() * XMMatrixTranslation(13.f, 0.77f, 8.f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Branch"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Collectible/Branch/CollectibleBranch.fbx", PreTransformMatrix))))
		return E_FAIL;

	/* For. Prototype_Component_Model_Flower*/
	PreTransformMatrix = XMMatrixIdentity() * XMMatrixTranslation(-1.f, 0.f, 15.f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Flower"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Collectible/Flower/CollectibleFlower.fbx", PreTransformMatrix))))
		return E_FAIL;

	/* For. Prototype_Component_Model_Stone*/
	PreTransformMatrix = XMMatrixIdentity() * XMMatrixTranslation(23.f, 1.6f, 31.f);
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Stone"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Collectible/Stone/CollectibleStone.fbx", PreTransformMatrix))))
		return E_FAIL;

	/* For. Prototype_Component_Model_Water*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Water"),
		CModel::Create(m_pDevice, m_pContext, CModel::TYPE_NONANIM, "../Bin/Resources/Models/Collectible/Water/CollectibleWater.fbx"))))
		return E_FAIL;

#pragma endregion

	lstrcpy(m_szLoadingText, TEXT("셰이더을(를) 로딩중입니다."));
	/* For. Prototype_Component_Shader_VtxNorTex*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxNorTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxNorTex.hlsl"), VTXNORTEX::Elements, VTXNORTEX::iNumElements))))
		return E_FAIL;

	/* For. Prototype_Component_Shader_VtxModel */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxModel"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxModel.hlsl"), VTXMESH::Elements, VTXMESH::iNumElements))))
		return E_FAIL;

	/* For. Prototype_Component_Shader_VtxAnimModel */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimModel"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxAnimModel.hlsl"), VTXANIMMESH::Elements, VTXANIMMESH::iNumElements))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("사운드을(를) 로딩중입니다."));


	lstrcpy(m_szLoadingText, TEXT("객체원형을(를) 로딩중입니다."));
	/* For. Prototype_GameObject_Terrain */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Terrain"),
		CTerrain::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	//For Prototype_GameObject_CustomNavi
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_CustomNavi"),
		CCustomNavi::Create(m_pDevice, m_pContext))))
		return E_FAIL;



	//For Prototype_GameObject_Map_Beach
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Map_Beach"),
		CMap_Beach::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	//For Prototype_GameObject_Map_Forest
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Map_Forest"),
		CMap_Forest::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	//For Prototype_GameObject_Map_Hotel
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Map_Hotel"),
		CMap_Hotel::Create(m_pDevice, m_pContext))))
		return E_FAIL;





	/* For. Prototype_GameObject_Monster */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster"),
		CMonster::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For. Prototype_GameObject_FreeCamera */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_FreeCamera"),
		CFreeCamera::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For. Prototype_GameObject_RotationCamera */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_RotationCamera"),
		CRotationCamera::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For. Prototype_GameObject_Beast */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Beast"),
		CBeast::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For. Prototype_GameObject_ItemBox */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ItemBox"),
		CItemBox::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	/* For. Prototype_GameObject_ItemBox_Beach */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ItemBox_Beach"),
		CItemBox_Beach::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For. Prototype_GameObject_Collectible */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Collectible"),
		CCollectible::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For. Prototype_GameObject_CollectibleUI */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_CollectibleUI"),
		CCollectibleUI::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	m_isFinished = true;

	return S_OK;
}

CLoader * CLoader::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, LEVELID eNextLevelID)
{
	CLoader*		pInstance = new CLoader(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevelID)))
	{
		MSG_BOX(TEXT("Failed to Created : CLoader"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoader::Free()
{
	__super::Free();

	WaitForSingleObject(m_hThread, INFINITE);

	DeleteObject(m_hThread);

	CloseHandle(m_hThread);

	DeleteCriticalSection(&m_CriticalSection);

	Safe_Release(m_pGameInstance);
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
