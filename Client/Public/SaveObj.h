#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Client)

class CSaveObj abstract :
	public CGameObject
{
public:
	struct SAVEOBJ_DESC : GAMEOBJECT_DESC
	{
		OBJTYPE eType;
		_int eMonsterID;
		_float3 vPos;
		_float3 vRotation;
	};

protected:
	CSaveObj(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	CSaveObj(const CSaveObj& Prototype);
	virtual ~CSaveObj() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override = 0;
	virtual void Update(_float fTimeDelta) override = 0;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override = 0;

public:
	void Set_Pos(_float3 _vPos);
	void Set_PosAll(_float _fPos);
	void Set_Scale(_float3 _vScale);
	void Set_RotateX(_float _fTheta);
	void Set_RotateY(_float _fTheta);
	void Set_RotateZ(_float _fTheta);
	void Plus_Pos(_float3 _fInput);
	_float3 Get_Scale();
	_float3 Get_Pos();
	OBJTYPE Get_Type() { return m_eType; }
	virtual int Get_ID() = 0;

private:
	OBJTYPE m_eType{ OBJ_COLLECTIBLE };

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END