#pragma once

#include "Client_Defines.h"


#include "Level.h"

BEGIN(Client)
class CBeast;
class CSaveObj;
class CCollectible;
class CLevel_GamePlay final : public CLevel
{
private:
	CLevel_GamePlay(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_GamePlay() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Ready_Lights();
	HRESULT Ready_Layer_Camera();	
	HRESULT Ready_Layer_BackGround();
	HRESULT Ready_Layer_Effect();

	HRESULT Ready_Layer_Monster();

	HRESULT Ready_ItemBox();

	HRESULT Ready_LandObjects();

	HRESULT Ready_SpecularMap();
	
private:
	_bool m_bGuiStart{ false };
	_bool m_bShow_Creating_Window{ false };
	_bool m_bShow_Setting_Window{ false };
	_bool m_bShow_Terrain_UI{ false };
	_bool m_bShow_Collectible_Obj{ false };

private:
	vector<CBeast*> m_vecMonster;
	vector<CCollectible*> m_vecCollectible;
	vector<CSaveObj*> m_vecMonsterGroup;
//	vector<CSaveCollectible*> m_vecCollectible;

	CSaveObj* m_pCurMonster{ nullptr };
	_float m_vRotation[3]{ 0.f, 0.f, 0.f };
	_float m_vRotateALL{ 0.f };
	_bool m_bRotateAll{ false };
	_float m_vPos[3]{ 0.f, 0.f, 0.f };
	_float m_vInput[3]{ 0.f, 0.f, 0.f };
	_int iTerrainInput[2] = { 0, 0 };
	_int iInput{ 0 };

private:
	void Format_ImGui();
	void Format_CreatingWindow();
	void Format_SettingWindow();
	void Format_GroupMove();
	void Format_Terrain_UI();
	void Format_CollectibleOBj();

private:
	void Key_Input();

private:
	void Syncro_CurMonster();
	void Attatch_On_Picking();
	const char* MonsterID_To_Char(_int _eMonsterID);
	const char* CollectibleID_To_Char(_int _eCollectibleID);
	void Load_Savemonster(ifstream* _LoadStream);


public:
	static CLevel_GamePlay* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END
