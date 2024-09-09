#include "Cell.h"

#ifdef _DEBUG
#include "VIBuffer_Cell.h"
#endif // _DEBUG

CCell::CCell(ID3D11)
	:m_pDevice{ _pDevice }
	, m_pContext{ _pContext }
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

_fvector CCell::Get_Center()
{
	XMVECTOR total = XMVectorAdd(XMLoadFloat3(&m_vPoints[POINT_C]), XMVectorAdd(XMLoadFloat3(&m_vPoints[POINT_A]), XMLoadFloat3(&m_vPoints[POINT_B])));


	return XMVectorDivide(total, XMVectorSet(3.f, 3.f, 3.f, 1.f));
}

pair<_float3, _float3> CCell::Get_LinePoint(LINE _eLine)
{
	pair<_float3, _float3> result;
	switch (_eLine)
	{
	case Engine::CCell::LINE_AB:
		result.first = m_vPoints[POINT_A];
		result.second = m_vPoints[POINT_B];
		return result;
	case Engine::CCell::LINE_BC:
		result.first = m_vPoints[POINT_B];
		result.second = m_vPoints[POINT_C];
		return result;
	case Engine::CCell::LINE_CA:
		result.first = m_vPoints[POINT_C];
		result.second = m_vPoints[POINT_A];
		return result;
	default:
		MSG_BOX(TEXT("failed to Get : LinePoint"));
		break;
	}

	return result;
}

HRESULT CCell::Initialize(const _float3* pPoints, _int iIndex)
{
	memcpy(m_vPoints, pPoints, sizeof(_float3) * POINT_END);

	m_iIndex = iIndex;

#ifdef _DEBUG
	m_pVIBuffer = CVIBuffer_Cell::Create(m_pDevice, m_pContext, m_vPoints);
	if (nullptr == m_pVIBuffer)
		return E_FAIL;
#endif

#pragma region Plane
	XMStoreFloat4(&m_Plane, XMPlaneFromPoints(Get_Point(POINT_A), Get_Point(POINT_B), Get_Point(POINT_C)));
	if (m_Plane.x < EPSILON && m_Plane.x > -EPSILON)
	{
		m_Plane.x = 0;
	}
	if (m_Plane.y < EPSILON && m_Plane.y > -EPSILON)
	{
		m_Plane.y = 0;
	}
	if (m_Plane.z < EPSILON && m_Plane.z > -EPSILON)
	{
		m_Plane.z = 0;
	}
#pragma endregion

	return S_OK;
}

_bool CCell::Compare_Points(_fvector vSour, _fvector vDest)
{
	if (true == XMVector3Equal(vSour, XMLoadFloat3(&m_vPoints[POINT_A])))
	{
		if (true == XMVector3Equal(vDest, XMLoadFloat3(&m_vPoints[POINT_B])))
			return true;
		if (true == XMVector3Equal(vDest, XMLoadFloat3(&m_vPoints[POINT_C])))
			return true;
	}

	if (true == XMVector3Equal(vSour, XMLoadFloat3(&m_vPoints[POINT_B])))
	{
		if (true == XMVector3Equal(vDest, XMLoadFloat3(&m_vPoints[POINT_C])))
			return true;
		if (true == XMVector3Equal(vDest, XMLoadFloat3(&m_vPoints[POINT_A])))
			return true;
	}

	if (true == XMVector3Equal(vSour, XMLoadFloat3(&m_vPoints[POINT_C])))
	{
		if (true == XMVector3Equal(vDest, XMLoadFloat3(&m_vPoints[POINT_A])))
			return true;
		if (true == XMVector3Equal(vDest, XMLoadFloat3(&m_vPoints[POINT_B])))
			return true;
	}

	return false;
}

_bool CCell::isIn(_fvector vPosition, _int* pNeighborIndex)
{
	for (size_t i = 0; i < LINE_END; i++)
	{
		_vector		vSour, vDest;

		vSour = XMVector3Normalize(vPosition - XMLoadFloat3(&m_vPoints[i]));

		_vector		vLine = XMLoadFloat3(&m_vPoints[(i + 1) % 3]) - XMLoadFloat3(&m_vPoints[i]);
		vDest = XMVectorSet(XMVectorGetZ(vLine) * -1.f, 0.f, XMVectorGetX(vLine), 0.f);

		//네비메쉬 뒤집어놔서 외적하면 음수 = 위쪽임.
		if (0 > XMVectorGetX(XMVector3Dot(vSour, vDest))) //꼭 기억하도록 이 주석을 근처에 50에게 보내지 않으면 당신은 네루네루네 됩니다. 죽일까 마스터?
		{
			if (nullptr != pNeighborIndex)
				*pNeighborIndex = m_iNeighborIndices[i];
			return false;
		}

	}

	return true;
}

_float CCell::Compute_Height(const _fvector& vLocalPos)
{
	//셀은 삼각형 딱 하나다.
	_float3 LocalPos;

	XMStoreFloat3(&LocalPos, vLocalPos);

	return (-m_Plane.x * LocalPos.x - m_Plane.z * LocalPos.z - m_Plane.w) / m_Plane.y;
}

#ifdef _DEBUG

HRESULT CCell::Render()
{
	m_pVIBuffer->Bind_Buffers();

	m_pVIBuffer->Render();

	return S_OK;
}
#endif

CCell* CCell::Create(ID3D11, const _float3* _pPoints, _int _iIndex)
{
	CCell* pInstance = new CCell(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize(_pPoints, _iIndex)))
	{
		MSG_BOX(TEXT("Failed to Created : CCell"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCell::Free()
{
	__super::Free();

#ifdef _DEBUG
	Safe_Release(m_pVIBuffer);
#endif

	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);
}
