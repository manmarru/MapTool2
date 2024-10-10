#pragma once

#include "../ImGui/imgui.h"
#include "../ImGui/imgui_impl_dx11.h"
#include "../ImGui/imgui_impl_win32.h"
#include <process.h>
#include "GameObject.h"

namespace Client
{
	const unsigned int			g_iWinSizeX = { 1280 };
	const unsigned int			g_iWinSizeY = { 720 };

	enum LEVELID { LEVEL_STATIC, LEVEL_LOADING, LEVEL_LOGO, LEVEL_GAMEPLAY, LEVEL_END };
	enum OBJTYPE
	{
		OBJ_BEAST
		, OBJ_COLLECTIBLE
		, OBJ_ITEMBOX
		, OBJ_END
	};

	enum SAVEMONSTERID
	{
		SAVEMONSTER_BAT
		, SAVEMONSTER_BEAR
		, SAVEMONSTER_BOAR
		, SAVEMONTSER_CHICKEN
		, SAVEMONSTER_DOG
		, SAVEMONSTER_WOLF

		, SAVEMONSTER_END
	};

	enum COLLECTIBLEID
	{
		SAVECOLLECTIBLE_STONE
		, SAVECOLLECTIBLE_FLOWER
		, SAVECOLLECTIBLE_WATER
		, SAVECOLLECTIBLE_BRANCH

		, SAVECOLLECTIBLE_END

	};

	enum ITEMID
	{
		ITEM_NONE = -1
		, ITEM_SODA
		, ITEM_POTATO
		, ITEM_GARLIC
		, ITEM_BREAD
		, ITEM_MEAT
		, ITEM_BUTTER
		, ITEM_WHISKEY//������	����

		, ITEM_GARLIC_BACON //	�ʷ�
		, ITEM_BURGER

		, ITEM_FRIED_POTATO
		, ITEM_HIGHBALL //		�Ķ�

		, ITEM_ELEVENSET //		���


		, ITEM_RUSTY_SWORD //	����
		, ITEM_PICKAXE // ���
		, ITEM_BRANCH //��������
		, ITEM_STONE	//��
		, ITEM_HEADBAND //�Ӹ���
		, ITEM_SHIRT //��
		, ITEM_BANDAGE	//�ش�
		, ITEM_BRACELET //����
		, ITEM_FEATHER //����
		, ITEM_LEATHER //����
		, ITEM_RUBBER //��
		, ITEM_SCRAP_METAL //��ö
		, ITEM_TOKEN //����
		, ITEM_PAPER //����
		, ITEM_SNEAKERS //�ȭ
		, ITEM_ORE //����
		, ITEM_CHAIN //��罽
		, ITEM_BOW //Ȱ

		, ITEM_E_COMPONENTS //�ʷ�
		, ITEM_GOLD //��
		, ITEM_BRACER //�극�̼� - �ʷ����
		, ITEM_BOOTS //����
		, ITEM_MASK //����
		, ITEM_LEATHER_ARMOR //���װ���
		, ITEM_LONGBOW //���	- �������

		, ITEM_GOLD_BRACELET //�Ķ�
		, ITEM_HELMET
		, ITEM_CHAINMAIL
		, ITEM_MOTOR
		, ITEM_DAGGER

		, ITEM_LIFETREE //����
		, ITEM_TONFA		//������� �ٽ� ���
		, ITEM_FINALBOW
		, ITEM_OPS
		, ITEM_BURGENET
		, ITEM_COMMANDER_ARMOR
		, ITEM_EOD
		, ITEM_DRAUPNIR
		, ITEM_SMART_WATCH
		, ITEM_KLINGON_BOOTS
		, ITEM_TACHYON
		, ITEM_GAUNTLET

		, ITEM_AMULET //Ȳ��

		, ITEM_END
	};

	enum ITEMBOXID
	{
		ITEMBOX_ICEBOX
		, ITEMBOX_DRUMBOX

		, ITEMBOX_END
	};
}

extern HINSTANCE g_hInst;
extern HWND g_hWnd;

using namespace Client;
using namespace std;
