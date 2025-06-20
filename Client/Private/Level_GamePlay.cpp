#include "stdafx.h"
#include "..\Public\Level_GamePlay.h"
#include "fstream"

#include "FreeCamera.h"
#include "terrain.h"
#include "Beast.h"
#include "ItemBox.h"
#include "Collectible.h"

#include "GameInstance.h"

CLevel_GamePlay::CLevel_GamePlay(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel{ pDevice, pContext }
{
}

HRESULT CLevel_GamePlay::Initialize()
{
	if (FAILED(Ready_Lights()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera()))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Effect()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Monster()))
		return E_FAIL;
	
	if (FAILED(Ready_ItemBox()))
		return E_FAIL;

	if (FAILED(Ready_LandObjects()))
		return E_FAIL;

	if (FAILED(Ready_SpecularMap()))
		return E_FAIL;

	// ImGui 초기화
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	// ImGui 스타일 고르기
	ImGui::StyleColorsDark(); // 다크 모드
	//ImGui::StyleColorsLight(); // 일반 모드

	ImGui_ImplWin32_Init(g_hWnd);
	ImGui_ImplDX11_Init(m_pDevice, m_pContext);

	return S_OK;
}

void CLevel_GamePlay::Update(_float fTimeDelta)
{
	Format_ImGui();

	if (m_bShow_Terrain_UI)
		Format_Terrain_UI();
	if (m_bShow_Creating_Window)
		Format_CreatingWindow();
	if (nullptr != m_pCurMonster)
		Format_SettingWindow();
	if (nullptr != m_pCurItemBox && m_bShow_ItemBox_Window)
		Format_ItemBoxSetting();
	if (!m_vecMonsterGroup.empty())
		Format_GroupMove();
	if (m_bShow_Collectible_Obj)
		Format_CollectibleOBj();

	Key_Input();
}

HRESULT CLevel_GamePlay::Render()
{
	SetWindowText(g_hWnd, TEXT("게임플레이레벨입니다."));

	if (!m_bGuiStart)
	{
		m_bGuiStart = true;
		return S_OK;
	}
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	return S_OK;
}


HRESULT CLevel_GamePlay::Ready_Lights()
{
	/* 게임플레이 레벨에 필요한 광원을 준비한다. */
	

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Camera()
{
	CFreeCamera::CAMERA_FREE_DESC		Desc{};

	Desc.fSensor = 0.2f;
	Desc.vEye = _float4(0.f, 10.f, -10.f, 1.f);
	Desc.vAt = _float4(0.f, 0.f, 0.f, 1.f);
	Desc.fFovy = XMConvertToRadians(60.0f);
	Desc.fNear = 0.1f;
	Desc.fFar = 1000.f;
	Desc.fSpeedPerSec = 30.f;
	Desc.fRotationPerSec = XMConvertToRadians(90.0f);
	Desc.fAspect = (_float)g_iWinSizeX / g_iWinSizeY;

	if (FAILED(m_pGameInstance->Add_CloneObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"), TEXT("Prototype_GameObject_FreeCamera"), &Desc)))
		return E_FAIL;
	m_pGameInstance->Insert_Camera(1, m_pGameInstance->Get_Objectlist(LEVEL_GAMEPLAY, TEXT("Layer_Camera"))->back());

	if (FAILED(m_pGameInstance->Add_CloneObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"), TEXT("Prototype_GameObject_RotationCamera"), &Desc)))
		return E_FAIL;

	m_pGameInstance->Insert_Camera(2, m_pGameInstance->Get_Objectlist(LEVEL_GAMEPLAY, TEXT("Layer_Camera"))->back());

	m_pGameInstance->Set_CameraIndex(1);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_BackGround()
{
	if (FAILED(m_pGameInstance->Add_CloneObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Layer_Terrain"), TEXT("Prototype_GameObject_Terrain"))))
		return E_FAIL;

	//네비메쉬 모델 출력용
	//if (FAILED(m_pGameInstance->Add_CloneObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Layer_Terrain"), TEXT("Prototype_GameObject_CustomNavi"))))
	//	return E_FAIL;



	//if (FAILED(m_pGameInstance->Add_CloneObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Layer_ModelMap"), TEXT("Prototype_GameObject_Map_Beach"))))
	//	return E_FAIL;

	//if (FAILED(m_pGameInstance->Add_CloneObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Layer_ModelMap"), TEXT("Prototype_GameObject_Map_Forest"))))
	//	return E_FAIL;

	if (FAILED(m_pGameInstance->Add_CloneObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Layer_ModelMap"), TEXT("Prototype_GameObject_Map_Hotel"))))
		return E_FAIL;



	//if (FAILED(m_pGameInstance->Add_CloneObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Layer_BackGround"), TEXT("Prototype_GameObject_Sky"))))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Effect()
{
	//for (size_t i = 0; i < 50; i++)
	//{
	//	if (FAILED(m_pGameInstance->Add_CloneObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Layer_BackGround"), TEXT("Prototype_GameObject_Effect"))))
	//		return E_FAIL;
	//}
	

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Monster()
{
	//if (FAILED(m_pGameInstance->Add_CloneObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Layer_Monster"), TEXT("Prototype_GameObject_Monster"))))
	//		return E_FAIL;	

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_ItemBox()
{
	wchar_t buffer[MAX_PATH];

	CItemBox::ITEMBOX_DESC itembox_Desc;


	for (size_t i = 1; i < 30; i++)
	{
		swprintf(buffer, MAX_PATH, TEXT("Prototype_Component_Model_ItemSpot_%d"), i);
		
		itembox_Desc.ModelTag = buffer;

		if (FAILED(m_pGameInstance->Add_CloneObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Layer_ItemBox"), TEXT("Prototype_GameObject_ItemBox"), &itembox_Desc)))
			return E_FAIL;
	}

	for (size_t i = 30; i < 58; i++)
	{
		swprintf(buffer, MAX_PATH, TEXT("Prototype_Component_Model_ItemSpot_%d"), i);

		itembox_Desc.ModelTag = buffer;

		if (FAILED(m_pGameInstance->Add_CloneObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Layer_ItemBox"), TEXT("Prototype_GameObject_ItemBox_Beach"), &itembox_Desc)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_LandObjects()
{
//	CLandObject::LANDOBJECT_DESC	Desc = {};
//
//	Desc.pTerrainTransform = dynamic_cast<CTransform*>(m_pGameInstance->Find_Component(LEVEL_GAMEPLAY, TEXT("Layer_BackGround"), TEXT("Com_Transform")));
//	Desc.pTerrainVIBuffer = dynamic_cast<CVIBuffer_Terrain*>(m_pGameInstance->Find_Component(LEVEL_GAMEPLAY, TEXT("Layer_BackGround"), TEXT("Com_VIBuffer")));
//
//	if (FAILED(Ready_Layer_Player(Desc)))
//		return E_FAIL;
//
//	if (FAILED(Ready_Layer_Monster(Desc)))
//		return E_FAIL;
//
	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_SpecularMap()
{
	ID3D11Texture2D* pTexture2D = { nullptr };

	D3D11_TEXTURE2D_DESC TextureDesc{};
	ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	_uint iWidth = 128; //밉맵크기맞춰라
	_uint iHeight = 128;


	TextureDesc.Width = iWidth;
	TextureDesc.Height = iHeight;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.SampleDesc.Count = 1;

	TextureDesc.Usage = D3D11_USAGE_STAGING;
	TextureDesc.BindFlags = 0;

	TextureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
	TextureDesc.MiscFlags = 0;

	_uint* pPixel = new _uint[TextureDesc.Width * TextureDesc.Height];
	ZeroMemory(pPixel, sizeof(_uint) * TextureDesc.Width * TextureDesc.Height);

	D3D11_SUBRESOURCE_DATA InitialDesc{};
	InitialDesc.pSysMem = pPixel;
	InitialDesc.SysMemPitch = TextureDesc.Width * sizeof(_uint);

	if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, &InitialDesc, &pTexture2D)))
		return E_FAIL;

	D3D11_MAPPED_SUBRESOURCE SubResource{};

	m_pContext->Map(pTexture2D, 0, D3D11_MAP_READ_WRITE, 0, &SubResource);

	for (size_t i = 0; i < iHeight; i++)
	{
		for (size_t j = 0; j < iWidth; j++)
		{
			_uint iIndex = i * iWidth + j;

			if (j < iWidth / 2)
				((_uint*)SubResource.pData)[iIndex] = D3DCOLOR_ARGB(255, 0, 0, 0);
			else
				((_uint*)SubResource.pData)[iIndex] = D3DCOLOR_ARGB(255, 255, 255, 255);
		}
	}

	m_pContext->Unmap(pTexture2D, 0);

	if (FAILED(SaveDDSTextureToFile(m_pContext, pTexture2D, TEXT("../Bin/Resources/Textures/Terrain/NeriSpecular.dds"))))
		return E_FAIL;

	Safe_Delete(pPixel);

	return S_OK;
}


//초기UI
void CLevel_GamePlay::Format_ImGui()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::Begin("Hello_World");
	if (ImGui::Button("Save_Monster"))
	{
		ofstream SaveStream("../Bin/Data/Monsters.txt");

		for (auto monster : m_vecMonster)
		{
			SaveStream << OBJ_BEAST << '\n' << monster->Get_ID() << '\n';

			SaveStream << monster->Get_Pos().x << ' ' << monster->Get_Pos().y << ' ' << monster->Get_Pos().z << '\n';
			
			SaveStream << monster->Get_Rotation().x << ' ' << monster->Get_Rotation().y << ' ' << monster->Get_Rotation().z << '\n';

			SaveStream << static_cast<CTerrain*>(m_pGameInstance->Get_Objectlist(LEVEL_GAMEPLAY, TEXT("Layer_Terrain"))->back())->Find_CellIndex(monster->Get_Pos()) << '\n';

			vector<ITEMID>* pvecInven = monster->Get_Inventory();
			for (auto item : *pvecInven)
				SaveStream << item << ' ';

			SaveStream << ITEM_NONE << '\n';
		}

		SaveStream.close();
	}

	ImGui::SameLine();

	if (ImGui::Button("Load_Monster"))
	{
		ifstream LoadStream("../Bin/Data/Monsters.txt", ios::out);
		_uint eObjType(OBJ_END);

		while (!LoadStream.eof())
		{

			LoadStream >> eObjType;
			if (OBJ_BEAST == eObjType)
			{
				Load_Savemonster(&LoadStream);
			}
		}
		LoadStream.close();
	}

	if (ImGui::Button("Clear monster"))
	{
		m_pCurMonster = nullptr;
		for (auto iter = m_vecMonster.begin(); iter != m_vecMonster.end();)
		{
			Safe_Release(*iter);
			iter = m_vecMonster.erase(iter);
		}
	}

	if (ImGui::Button("Remake_Terrain"))
	{
		m_bShow_Terrain_UI = true;
	}

	if (ImGui::Button("Collectible"))
	{
		m_bShow_Collectible_Obj = true;
	}

	if (ImGui::Button("ItemBox"))
		m_bShow_ItemBox_Window = true;

	ImGui::Checkbox("Creating", &m_bShow_Creating_Window);

	ImGui::End();

	return;
}

void CLevel_GamePlay::Format_CreatingWindow()
{
	ImGui::Begin("Creating", &m_bShow_Creating_Window);

#pragma region 몬스터 생성 버튼
	if (ImGui::Button("Bat"))
	{
		CBeast::SAVEOBJ_DESC Desc;
		Desc.fRotationPerSec = 0.f; Desc.fSpeedPerSec = 0.f;
		Desc.eMonsterID = SAVEMONSTER_BAT;
		Desc.vPos = _float3{ 0.f, 0.f, 0.f };
		Desc.vRotation = _float3{ 1.f,1.f,1.f };
		Desc.eType = OBJ_BEAST;
		CBeast* pmonster = static_cast<CBeast*>(m_pGameInstance->Add_CloneObject_ToLayer_Get(LEVEL_GAMEPLAY, TEXT("Layer_Beast"), TEXT("Prototype_GameObject_Beast"), &Desc));
		m_vecMonster.push_back(pmonster);
	}
	ImGui::SameLine();
	if (ImGui::Button("Bear"))
	{
		CBeast::SAVEOBJ_DESC Desc;
		Desc.fRotationPerSec = 0.f; Desc.fSpeedPerSec = 0.f;
		Desc.eMonsterID = SAVEMONSTER_BEAR;
		Desc.vPos = _float3{ 0.f, 0.f, 0.f };
		Desc.vRotation = _float3{ 1.f,1.f,1.f };
		Desc.eType = OBJ_BEAST;
		CBeast* pmonster = static_cast<CBeast*>(m_pGameInstance->Add_CloneObject_ToLayer_Get(LEVEL_GAMEPLAY, TEXT("Layer_Beast"), TEXT("Prototype_GameObject_Beast"), &Desc));
		m_vecMonster.push_back(pmonster);
	}
	ImGui::SameLine();
	if (ImGui::Button("Boar"))
	{
		CBeast::SAVEOBJ_DESC Desc;
		Desc.fRotationPerSec = 0.f; Desc.fSpeedPerSec = 0.f;
		Desc.eMonsterID = SAVEMONSTER_BOAR;
		Desc.vPos = _float3{ 0.f, 0.f, 0.f };
		Desc.vRotation = _float3{ 1.f,1.f,1.f };
		Desc.eType = OBJ_BEAST;
		CBeast* pmonster = static_cast<CBeast*>(m_pGameInstance->Add_CloneObject_ToLayer_Get(LEVEL_GAMEPLAY, TEXT("Layer_Beast"), TEXT("Prototype_GameObject_Beast"), &Desc));
		m_vecMonster.push_back(pmonster);
	}
	if (ImGui::Button("Chicken"))
	{
		CBeast::SAVEOBJ_DESC Desc;// = { 0.f, 0.f, SAVEMONSTER_BEAR, _float3{0.f, 0.f, 0.f}, _float3{1.f,1.f,1.f} };
		Desc.fRotationPerSec = 0.f; Desc.fSpeedPerSec = 0.f;
		Desc.eMonsterID = SAVEMONTSER_CHICKEN;
		Desc.vPos = _float3{ 0.f, 0.f, 0.f };
		Desc.vRotation = _float3{ 1.f,1.f,1.f };
		Desc.eType = OBJ_BEAST;
		CBeast* pmonster = static_cast<CBeast*>(m_pGameInstance->Add_CloneObject_ToLayer_Get(LEVEL_GAMEPLAY, TEXT("Layer_Beast"), TEXT("Prototype_GameObject_Beast"), &Desc));
		m_vecMonster.push_back(pmonster);
	}
	ImGui::SameLine();
	if (ImGui::Button("Dog"))
	{
		CBeast::SAVEOBJ_DESC Desc;// = { 0.f, 0.f, SAVEMONSTER_BEAR, _float3{0.f, 0.f, 0.f}, _float3{1.f,1.f,1.f} };
		Desc.fRotationPerSec = 0.f; Desc.fSpeedPerSec = 0.f;
		Desc.eMonsterID = SAVEMONSTER_DOG;
		Desc.vPos = _float3{ 0.f, 0.f, 0.f };
		Desc.vRotation = _float3{ 1.f,1.f,1.f };
		Desc.eType = OBJ_BEAST;
		CBeast* pmonster = static_cast<CBeast*>(m_pGameInstance->Add_CloneObject_ToLayer_Get(LEVEL_GAMEPLAY, TEXT("Layer_Beast"), TEXT("Prototype_GameObject_Beast"), &Desc));
		m_vecMonster.push_back(pmonster);
	}
	ImGui::SameLine();
	if (ImGui::Button("Wolf"))
	{
		CBeast::SAVEOBJ_DESC Desc;// = { 0.f, 0.f, SAVEMONSTER_BEAR, _float3{0.f, 0.f, 0.f}, _float3{1.f,1.f,1.f} };
		Desc.fRotationPerSec = 0.f; Desc.fSpeedPerSec = 0.f;
		Desc.eMonsterID = SAVEMONSTER_WOLF;
		Desc.vPos = _float3{ 0.f, 0.f, 0.f };
		Desc.vRotation = _float3{ 1.f,1.f,1.f };
		Desc.eType = OBJ_BEAST;
		CBeast* pmonster = static_cast<CBeast*>(m_pGameInstance->Add_CloneObject_ToLayer_Get(LEVEL_GAMEPLAY, TEXT("Layer_Beast"), TEXT("Prototype_GameObject_Beast"), &Desc));
		m_vecMonster.push_back(pmonster);
	}
#pragma endregion

	if (!m_vecMonster.empty())
	{
		bool temp = ImGui::BeginTable("Test", 1);
		for (int i = 0; i < m_vecMonster.size(); ++i)
		{
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			char buf[128];
			CBeast* pBeast = m_vecMonster[i];
			sprintf_s(buf, "%s : (%.2f, %.2f, %.2f) ##%d", MonsterID_To_Char(pBeast->Get_ID()),
				pBeast->Get_Pos().x, pBeast->Get_Pos().y, pBeast->Get_Pos().z, i);
			if (ImGui::Button(buf))
			{
				m_pCurMonster = m_vecMonster[i];
				m_pGameInstance->Set_Target(m_vecMonster[i]->Get_Pos());
				Syncro_CurMonster();
			}
		}

		if (temp)
			ImGui::EndTable();

	}
	ImGui::End();
}

void CLevel_GamePlay::Format_SettingWindow()
{
	ImGui::Begin("Setting");

	ImGui::BulletText("Rotation");

	ImGui::SliderFloat3("xyzRotation", m_vRotation, 0, 360, "%.3f", 1);
	ImGui::Checkbox("syncro", &m_bRotateAll);
	if (m_bRotateAll)
	{
		m_pCurMonster->Set_Rotation(XMConvertToRadians(m_vRotation[0]), XMConvertToRadians(m_vRotation[1]), XMConvertToRadians(m_vRotation[2]));
	}

	/*m_pCurMonster->Set_RotateX(m_vRotation[0]);
	m_pCurMonster->Set_RotateY(m_vRotation[1]);
	m_pCurMonster->Set_RotateZ(m_vRotation[2]);*/

	//ImGui::ColorEdit3("clear color", (float*)&clear_color);

	ImGui::Text("Move");
	ImGui::SliderFloat3("xyzMoving", m_vPos, 0, 100, "%.3f", 1.0f);
	if (ImGui::Button("Attach on Terrain"))
		m_vPos[1] = 0;

	m_pCurMonster->Set_Scale(_float3{ m_vRotation });
	m_pCurMonster->Set_Pos(_float3{ m_vPos });

	if (ImGui::Button("Add Group"))
	{
		m_vecMonsterGroup.push_back(m_pCurMonster);
		m_pCurMonster = nullptr;
		ImGui::End();
		return;

	}

	if (OBJ_BEAST == m_pCurMonster->Get_Type())
	{
		//인벤토리
		ImGui::Text("Inventory");
		ImGui::InputInt("AddItem", &iInput);
		ImGui::SameLine();
		if (ImGui::Button("Add"))
		{
			((CBeast*)m_pCurMonster)->Add_Item((ITEMID)iInput);
		}

		_bool invenHr = ImGui::BeginTable("inventoryTable", 4);
		vector<ITEMID>* pinven = ((CBeast*)m_pCurMonster)->Get_Inventory();

		int i(0);

		for (auto iter = pinven->begin(); iter != pinven->end();)
		{
			ImGui::TableNextColumn();
			char buf[128];
			//sprintf_s(buf, "%d ##%d", (int)(*iter), i);
			sprintf_s(buf, "%d ##%d", (_int)*iter, i);
			if (ImGui::Button(buf))
			{
				iter = pinven->erase(iter);
			}
			else
				++iter;
			++i;
		}
		if (invenHr)
			ImGui::EndTable();
	}

	if (GetAsyncKeyState('P'))
	{
		Attatch_On_Picking(); //ㅋㅋ 이걸 왜함 리소스 다있는데
	}

	ImGui::End();
}

void CLevel_GamePlay::Format_ItemBoxSetting()
{
	ImGui::Begin("Setting", &m_bShow_ItemBox_Window);
	//인벤토리
	ImGui::Text("Inventory");

	ImGui::SameLine();
	if (ImGui::Button("saveItemBox"))
	{
		Save_ItemBox();
	}

	ImGui::SameLine();
	if (ImGui::Button("LoadItemBox"))
	{
		Load_ItemBox();
	}

	ImGui::InputInt("AddItem", &iInput);
	ImGui::SameLine();
	if (ImGui::Button("Add"))
	{
		m_pCurItemBox->Item_Input((ITEMID)iInput);
	}

	_bool invenHr = ImGui::BeginTable("inventoryTable", 6);
	list<ITEMID>* pinven = m_pCurItemBox->Get_Inventory();

	int i(0);

	for (auto iter = pinven->begin(); iter != pinven->end();)
	{
		ImGui::TableNextColumn();
		char buf[128];
		//sprintf_s(buf, "%d ##%d", (int)(*iter), i);
		sprintf_s(buf, "%d ##%d", (int)(*iter), i);
		if (ImGui::Button(buf))
		{
			iter = pinven->erase(iter);
		}
		else
			++iter;
		++i;
	}
	if (invenHr)
		ImGui::EndTable();



	ImGui::End();
}

void CLevel_GamePlay::Format_GroupMove()
{
	ImGui::Begin("Group");

	ImGui::InputFloat3("Move", m_vInput);
	if (ImGui::Button("purchase"))
	{
		for (auto monster : m_vecMonsterGroup)
			monster->Plus_Pos(_float3{ m_vInput[0], m_vInput[1], m_vInput[2] });
	}

	if (!m_vecMonsterGroup.empty())
	{
		ImGui::BeginTable("Test", 1);
		for (int i = 0; i < m_vecMonsterGroup.size(); ++i)
		{
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			char buf[128];
			CSaveObj* pObj = m_vecMonsterGroup[i];
			sprintf_s(buf, "%s : (%.2f, %.2f, %.2f) ##%d", pObj->Get_Type() == OBJ_BEAST ? MonsterID_To_Char(pObj->Get_ID()) : CollectibleID_To_Char(pObj->Get_ID()),
				pObj->Get_Pos().x, pObj->Get_Pos().y, pObj->Get_Pos().z, i);
			if (ImGui::Button(buf))
			{
				m_vecMonsterGroup.erase(m_vecMonsterGroup.begin() + i);
			}
		}

		ImGui::EndTable();
	}


	ImGui::End();
}

void CLevel_GamePlay::Format_Terrain_UI()
{
	ImGui::Begin("Remake_Terrain", &m_bShow_Terrain_UI);

	ImGui::InputInt2("TerrainSize", iTerrainInput);
	vector<int> test;

	ImGui::End();
}

void CLevel_GamePlay::Format_CollectibleOBj()
{
	ImGui::Begin("Create_Collectible", &m_bShow_Collectible_Obj);

#pragma region 채집물

	if (ImGui::Button("Stone"))
	{
		CSaveObj::SAVEOBJ_DESC Desc;
		Desc.eMonsterID = SAVECOLLECTIBLE_STONE;
		Desc.vPos = { 0.f, 0.f, 0.f };
		Desc.fRotationPerSec = 0.f;
		Desc.fSpeedPerSec = 1.f;
		Desc.vRotation = { 1.f, 1.f, 1.f };
		Desc.eType = OBJ_COLLECTIBLE;
		m_vecCollectible.push_back((CCollectible*)m_pGameInstance->Add_CloneObject_ToLayer_Get(LEVEL_GAMEPLAY, TEXT("Layer_Collectible"), TEXT("Prototype_GameObject_Collectible"), &Desc));
	}
	ImGui::SameLine();

	if (ImGui::Button("Flower"))
	{
		CSaveObj::SAVEOBJ_DESC Desc;
		Desc.eMonsterID = SAVECOLLECTIBLE_FLOWER;
		Desc.vPos = { 0.f, 0.f, 0.f };
		Desc.fRotationPerSec = 0.f;
		Desc.fSpeedPerSec = 1.f;
		Desc.vRotation = { 1.f, 1.f, 1.f };
		Desc.eType = OBJ_COLLECTIBLE;
		m_vecCollectible.push_back((CCollectible*)m_pGameInstance->Add_CloneObject_ToLayer_Get(LEVEL_GAMEPLAY, TEXT("Layer_Collectible"), TEXT("Prototype_GameObject_Collectible"), &Desc));
	}
	ImGui::SameLine();

	if (ImGui::Button("Water"))
	{
		//CSaveObj::SAVEOBJ_DESC Desc;
		//Desc.eMonsterID = SAVECOLLECTIBLE_WATER;
		//Desc.vPos = { 0.f, 0.f, 0.f };
		//Desc.fRotationPerSec = 0.f;
		//Desc.fSpeedPerSec = 1.f;
		//Desc.vRotation = { 1.f, 1.f, 1.f };
		//Desc.eType = OBJ_COLLECTIBLE;
		//m_vecCollectible.push_back((CCollectible*)m_pGameInstance->Add_CloneObject_ToLayer_Get(LEVEL_GAMEPLAY, TEXT("Layer_Collectible"), TEXT("Prototype_GameObject_Collectible"), &Desc));
	}

	if (ImGui::Button("Branch"))
	{
		CSaveObj::SAVEOBJ_DESC Desc;
		Desc.eMonsterID = SAVECOLLECTIBLE_BRANCH;
		Desc.vPos = { 0.f, 0.f, 0.f };
		Desc.fRotationPerSec = 0.f;
		Desc.fSpeedPerSec = 1.f;
		Desc.vRotation = { 1.f, 1.f, 1.f };
		Desc.eType = OBJ_COLLECTIBLE;
		m_vecCollectible.push_back((CCollectible*)m_pGameInstance->Add_CloneObject_ToLayer_Get(LEVEL_GAMEPLAY, TEXT("Layer_Collectible"), TEXT("Prototype_GameObject_Collectible"), &Desc));
	}

#pragma endregion

	ImGui::BeginTable("Table_Collectibles", 1);
	if (!m_vecCollectible.empty())
	{
		for (int i = 0; i < m_vecCollectible.size(); ++i)
		{
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			char buf[128];
			CCollectible* pCollectible = m_vecCollectible[i];
			sprintf_s(buf, "%s : (%.2f, %.2f, %.2f) ##%d", CollectibleID_To_Char(pCollectible->Get_ID()),
				pCollectible->Get_Pos().x, pCollectible->Get_Pos().y, pCollectible->Get_Pos().z, i);
			if (ImGui::Button(buf))
			{
				m_pCurMonster = m_vecCollectible[i];
				Syncro_CurMonster();
				m_bShow_Setting_Window = true;
			}
		}
	}

	ImGui::EndTable();


	ImGui::End();
}


//표시되는 수치 동기화
void CLevel_GamePlay::Syncro_CurMonster()
{
	_float3 MonsterScale = m_pCurMonster->Get_Scale();
	_float3 MonsterPos = m_pCurMonster->Get_Pos();
	m_vRotation[0] = MonsterScale.x;
	m_vRotation[1] = MonsterScale.y;
	m_vRotation[2] = MonsterScale.z;

	m_vPos[0] = MonsterPos.x;
	m_vPos[1] = MonsterPos.y;
	m_vPos[2] = MonsterPos.z;
}

void CLevel_GamePlay::Attatch_On_Picking()
{
	CGameObject* pterrain = m_pGameInstance->Get_Objectlist(LEVEL_GAMEPLAY, TEXT("Layer_Terrain"))->back();
	_float4x4 temp = static_cast<CTerrain*>(pterrain)->Get_WorldMatrix();

	_float3 vPickPos{};

	if (true == static_cast<CTerrain*>(pterrain)->isPicking(&vPickPos));
	{
		m_pCurMonster->Set_Pos(_float3{ vPickPos.x, vPickPos.y, vPickPos.z });
		m_vPos[0] = vPickPos.x; m_vPos[1] = vPickPos.y; m_vPos[2] = vPickPos.z;
	}
}

void CLevel_GamePlay::Key_Input()
{
	if (GetAsyncKeyState(VK_RSHIFT))
	{
		m_pGameInstance->Set_CameraIndex(2);
	}

	if (GetAsyncKeyState(VK_F1))
	{
		int a = m_pGameInstance->ObjectPicking();
		if (0 != a)
			m_pCurItemBox = static_cast<CItemBox*>(m_pGameInstance->Find_Sygnature(a));
		
	}

}

const char* CLevel_GamePlay::MonsterID_To_Char(_int _eMonsterID)
{
	switch ((SAVEMONSTERID)_eMonsterID)
	{
	case Client::SAVEMONSTER_BAT:
		return "Bat";
	case Client::SAVEMONSTER_BEAR:
		return "Bear";
	case Client::SAVEMONSTER_BOAR:
		return "Boar";
	case Client::SAVEMONTSER_CHICKEN:
		return "Chicken";
	case Client::SAVEMONSTER_DOG:
		return "Dog";
	case Client::SAVEMONSTER_WOLF:
		return "Wolf";
	default:
		return "";
	}
	return "";
}

const char* CLevel_GamePlay::CollectibleID_To_Char(_int _eCollectibleID)
{
	switch ((COLLECTIBLEID)_eCollectibleID)
	{
	case Client::SAVECOLLECTIBLE_STONE:
		return "Stone";
	case Client::SAVECOLLECTIBLE_FLOWER:
		return "Flower";
	case Client::SAVECOLLECTIBLE_WATER:
		return "Water";
	case Client::SAVECOLLECTIBLE_BRANCH:
		return "Branch";
	default:
		return "";
	}
	return "";
}

void CLevel_GamePlay::Load_Savemonster(ifstream* _LoadStream)
{
	_int iOutput;
	_float3 vOutputPos;
	*_LoadStream >> iOutput;
	CSaveObj::SAVEOBJ_DESC Desc;
	Desc.eType = OBJ_BEAST;
	Desc.eMonsterID = SAVEMONSTERID(iOutput);

	if (_LoadStream->eof())
		return;

	*_LoadStream >> vOutputPos.x >> vOutputPos.y >> vOutputPos.z;
	Desc.vPos = vOutputPos;
	*_LoadStream >> vOutputPos.x >> vOutputPos.y >> vOutputPos.z;
	Desc.vRotation = vOutputPos;

	*_LoadStream >> Desc.iCellIndex;

	Desc.fRotationPerSec = 0.f; Desc.fSpeedPerSec = 0.f;

	CBeast* pMonster;
	pMonster = (CBeast*)m_pGameInstance->Add_CloneObject_ToLayer_Get(LEVEL_GAMEPLAY, TEXT("Layer_Beast"), TEXT("Prototype_GameObject_Beast"), &Desc);

	while (true)
	{
		int eID;
		*_LoadStream >> eID;
		if (ITEM_NONE == (ITEMID)eID)
			break;
		pMonster->Add_Item((ITEMID)eID);
	}

	if (_LoadStream->eof())
	{
		Safe_Release<CBeast*>(pMonster);
		return;
	}

	m_vecMonster.push_back(pMonster);
}

void CLevel_GamePlay::Save_ItemBox()
{
	ofstream SaveStream("../Bin/Data/ItemBoxInventory.txt", ios::trunc);

	ITEMID saveinventory[6];

	_int i(0);
	list<CGameObject*> itemboxes = *(m_pGameInstance->Get_Objectlist(LEVEL_GAMEPLAY, TEXT("Layer_ItemBox")));
	for (auto pObj : itemboxes)
	{
		memset(saveinventory, ITEM_NONE, sizeof(ITEMID) * 6);
		i = 0;
		for (auto item : *static_cast<CItemBox*>(pObj)->Get_Inventory())
		{
			saveinventory[i] = item;
			++i;
			if (i >= 6)
				break;
		}
		//SaveStream.write((const char*)(&saveinventory), sizeof(ITEMID) * 6);

		for (size_t i = 0; i < 6; i++)
		{
			SaveStream << saveinventory[i] << ' ';
		}
		SaveStream << endl;
	}

	SaveStream.close();
}

void CLevel_GamePlay::Load_ItemBox()
{
	ifstream LoadStream("../Bin/Data/ItemBoxInventory.txt");

	//ITEMID Loadinventory[6];
	_int Loadinventory[6];
	memset(Loadinventory, ITEM_NONE, sizeof(ITEMID) * 6);

	_int i(0);
	for (auto pObj : *m_pGameInstance->Get_Objectlist(LEVEL_GAMEPLAY, TEXT("Layer_ItemBox")))
	{
	
		//LoadStream.read((char*)Loadinventory, sizeof(ITEMID) * 6);
		for (size_t i = 0; i < 6; i++)
		{
			LoadStream >> Loadinventory[i];
		}

		for (size_t i = 0; i < 6; i++)
		{
			static_cast<CItemBox*>(pObj)->Item_Input((ITEMID)Loadinventory[i]);
		}
		memset(Loadinventory, ITEM_NONE, sizeof(ITEMID) * 6);
	}

	LoadStream.close();
}


CLevel_GamePlay * CLevel_GamePlay::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_GamePlay*		pInstance = new CLevel_GamePlay(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CLevel_GamePlay"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_GamePlay::Free()
{
	__super::Free();

	/*ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();*/
}