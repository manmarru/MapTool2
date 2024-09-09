#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CNavigation final :
	public CComponent
{
public:
	typedef struct
	{
		_int			iCurrentIndex = { -1 };
	}NAVIGATION_DESC;
private:
	CNavigation(ID3D11);
	CNavigation(const CNavigation& Prototype);
	virtual ~CNavigation() = default;

public:
	virtual HRESULT Initialize_Prototype(ifstream* _LoadStream, _fmatrix PreTransformMatrix);
	virtual HRESULT Initialize(void* pArg) override;
	void Update(_fmatrix TerrainWorldMatrix);
	_bool isMove(_fvector vPosition);
#ifdef _DEBUG
	HRESULT Render();
	void Rending_All();
#endif

public:
	_float4x4 Get_WorldMatrix() { return m_WorldMatrix; }
	_float4x4 Get_WorldMatrix_Inverse();
	_fvector Get_Center(_int _iCellIndex);
	_float Compute_Height(const _fvector& vLocalPos);
	_bool isPicking(_float3* pOut);
	_bool isCheckPoint(_int _iCheckPointIndex) { return m_iCurrentCellIndex == _iCheckPointIndex; }

private:
	vector<class CCell*>				m_Cells;
	_int								m_iCurrentCellIndex = { -1 };
	static _float4x4					m_WorldMatrix;

	//길찾기용
	list<_int>							m_listOpen;
	list<_int>							m_listHistory; // 밟았던 땅
	//stack<pair<_float3, _float3>>		m_StlLine; // 붙은선 스텍


#ifdef _DEBUG
private:
	class CShader* m_pShader = { nullptr };
#endif

private:
	_float4x4 m_PreTransformMatrix = {};
	DXGI_FORMAT					m_eIndexFormat = {};
	D3D11_PRIMITIVE_TOPOLOGY	m_eTopology = {};

private:
	HRESULT SetUp_Neighbors();
	HRESULT Ready_Cells(ifstream* _LoadStream);
	_bool CrossCheck(_float3 _vStart, _float3 _vEnd, _float3 _vP1, _float3 _vP2);
	_float3 Closer(_float3 _vMainPos, _float3 _vP1, _float3 _vP2);
	_float3 Brand_NEW_Closer(_float3 _vCompare1, _float3 _vCompare2, _float3 _vNextLineCenter);

public:
	_uint Find_MyCell(_fvector _WorldPos);



public:
	static CNavigation* Create(ID3D11, ifstream* _LoadStream, _fmatrix PreTransformMatrix);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END