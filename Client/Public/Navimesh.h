#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class Cmodel;
END

BEGIN(Client)
class CNavimesh :
    public CGameObject
{
private:
    CNavimesh(ID3D11);


};

END