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
		, ITEM_WHISKEY//¿©±îÁö	°ËÁ¤

		, ITEM_GARLIC_BACON //	ÃÊ·Ï
		, ITEM_BURGER

		, ITEM_FRIED_POTATO
		, ITEM_HIGHBALL //		ÆÄ¶û

		, ITEM_ELEVENSET //		³ë¶û


		, ITEM_RUSTY_SWORD //	°ËÁ¤
		, ITEM_PICKAXE // °î±ªÀÌ
		, ITEM_BRANCH //³ª¹µ°¡Áö
		, ITEM_STONE	//µ¹
		, ITEM_HEADBAND //¸Ó¸®¶ì
		, ITEM_SHIRT //¼Å
		, ITEM_BANDAGE	//ºØ´ë
		, ITEM_BRACELET //ÆÈÂî
		, ITEM_FEATHER //±êÅÐ
		, ITEM_LEATHER //°¡Á×
		, ITEM_RUBBER //°í¹«
		, ITEM_SCRAP_METAL //°íÃ¶
		, ITEM_TOKEN //¸¶ÆÐ
		, ITEM_PAPER //Á¾ÀÌ
		, ITEM_SNEAKERS //¿îµ¿È­
		, ITEM_ORE //±¤¼®
		, ITEM_CHAIN //¼è»ç½½
		, ITEM_BOW //È°

		, ITEM_E_COMPONENTS //ÃÊ·Ï
		, ITEM_GOLD //±Ý
		, ITEM_BRACER //ºê·¹ÀÌ¼­ - ÃÊ·ÏÀåºñ
		, ITEM_BOOTS //ºÎÃ÷
		, ITEM_MASK //°¡¸é
		, ITEM_LEATHER_ARMOR //°¡Á×°©¿Ê
		, ITEM_LONGBOW //Àå±Ã	- ¿©±â±îÁö

		, ITEM_GOLD_BRACELET //ÆÄ¶û
		, ITEM_HELMET
		, ITEM_CHAINMAIL
		, ITEM_MOTOR
		, ITEM_DAGGER

		, ITEM_LIFETREE //º¸¶ó
		, ITEM_TONFA		//¿©±âºÎÅÍ ´Ù½Ã Àåºñ
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

		, ITEM_AMULET //È²±Ý

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
