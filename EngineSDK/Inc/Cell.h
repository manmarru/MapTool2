#pragma once

#include "Base.h"

BEGIN(Engine)

class CCell final :
    public CBase
{
public:
	enum POINT { POINT_A, POINT_B, POINT_C, POINT_END };
	enum LINE { LINE_AB, LINE_BC, LINE_CA, LINE_END };

private:
	CCell(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
	virtual ~CCell() = default;
	
public:
	_vector Get_Point(POINT ePoint) const { return XMLoadFloat3(&m_vPoints[ePoint]); }
	void Set_Neighbor(LINE eLine, CCell* pNeighbor) { m_iNeighborIndices[eLine] = pNeighbor->m_iIndex; }

public:
	HRESULT Initialize(const _float3* pPoints, _int iIndex);
	_bool Compare_Points(_fvector vSour, _fvector vDest);
	_bool isIn(_fvector vPosition, _int* pNeighborIndex);

#ifdef _DEBUG
public:
	HRESULT Render();
#endif // _DEBUG


private:
	ID3D11Device*				m_pDevice = { nullptr };
	ID3D11DeviceContext*		m_pContext = { nullptr };
	_int						m_iIndex = {};
	_float3						m_vPoints[POINT_END] = {};
	_int						m_iNeighborIndices[LINE_END] = { -1, -1, -1 };

#ifdef _DEBUG
private:
	class CVIBuffer_Cell*		m_pVIBuffer = { nullptr };
#endif // _DEBUG


public:
	static CCell* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext, const _float3* pPoints, _int iIndex);
	virtual void Free() override;
};

END