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
		ITEM_CARERA
		, ITEM_SCOPE
		, ITEM_SEARCHDRONE
		, ITEM_GUNPOWDER

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
