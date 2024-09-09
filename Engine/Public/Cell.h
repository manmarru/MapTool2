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
    CCell(ID3D11);
    virtual ~CCell() = default;

public:
    _vector Get_Point(POINT ePoint) const { return XMLoadFloat3(&m_vPoints[ePoint]); }

    void Set_Neighbor(LINE eLine, CCell* pNeighbor) { m_iNeighborIndices[eLine] = pNeighbor->m_iIndex; }

    _float4 Get_Plane() { return m_Plane; }

    _int Get_Index() { return m_iIndex; }


    //길찾기용
    _int Get_ParentCellIndex() { return m_iParentCell; }
    _fvector Get_Center();
    _int* Get_Neighbors() { return m_iNeighborIndices; }
    void Set_ParentCell(_int _CellIndex) { m_iParentCell = _CellIndex; }
    pair<_float3, _float3> Get_LinePoint(LINE _eLine);

public:
    HRESULT Initialize(const _float3* pPoints, _int iIndex);
    _bool Compare_Points(_fvector vSour, _fvector vDest);
    _bool isIn(_fvector vPosition, _int* pNeighborIndex);
    _float Compute_Height(const _fvector& vLocalPos);

#ifdef _DEBUG
public:
    HRESULT Render();
#endif

private:
    ID3D11Device* m_pDevice = { nullptr };
    ID3D11DeviceContext* m_pContext = { nullptr };
    _int							m_iIndex = {};
    _float3							m_vPoints[POINT_END] = {};
    _int							m_iNeighborIndices[LINE_END] = { -1, -1, -1 };
    _float4                         m_Plane = {};

    //길찾기용
    _int m_iParentCell;


#ifdef _DEBUG
private:
    class CVIBuffer_Cell* m_pVIBuffer = { nullptr };
#endif


public:
    static CCell* Create(ID3D11, const _float3* _pPoints, _int _iIndex);
    virtual void Free() override;
};

END