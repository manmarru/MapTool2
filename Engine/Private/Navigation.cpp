#include "Navigation.h"
#include "Cell.h"

#include "Shader.h"
#include "GameInstance.h"
#include "Transform.h"

_float4x4 CNavigation::m_WorldMatrix = {};

CNavigation::CNavigation(ID3D11)
	:CComponent{ _pDevice, _pContext }
{
}

CNavigation::CNavigation(const CNavigation& Prototype)
	:CComponent(Prototype)
	, m_Cells{ Prototype.m_Cells }
#ifdef _DEBUG
	, m_pShader{ Prototype.m_pShader }
	, m_PreTransformMatrix{ Prototype.m_PreTransformMatrix }
#endif
{
	for (auto& pCell : m_Cells)
		Safe_AddRef(pCell);
#ifdef _DEBUG
	Safe_AddRef(m_pShader);
#endif
}

HRESULT CNavigation::Initialize_Prototype(ifstream* _LoadStream, _fmatrix PreTransformMatrix)
{
	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());

	XMStoreFloat4x4(&m_PreTransformMatrix, PreTransformMatrix);

	if (FAILED(Ready_Cells(_LoadStream)))
		return E_FAIL;

	if (FAILED(SetUp_Neighbors()))
		return E_FAIL;

#ifdef _DEBUG

	m_pShader = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Cell.hlsl"), VTXPOS::Elements, VTXPOS::iNumElements);
	if (nullptr == m_pShader)
		return E_FAIL;

#endif

	return S_OK;
}

HRESULT CNavigation::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return S_OK;

	NAVIGATION_DESC* pDesc = static_cast<NAVIGATION_DESC*>(pArg);

	m_iCurrentCellIndex = pDesc->iCurrentIndex;

	return S_OK;
}

void CNavigation::Update(_fmatrix TerrainWorldMatrix)
{
	XMStoreFloat4x4(&m_WorldMatrix, TerrainWorldMatrix);
}

_bool CNavigation::isMove(_fvector vPosition)
{
	_vector		vLocalPos = XMVector3TransformCoord(vPosition, XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_WorldMatrix)));

	_int			iNeighborIndex = { -1 };

	/* 원래 있던 삼각형 안에서 움직인거야. */
	if (true == m_Cells[m_iCurrentCellIndex]->isIn(vLocalPos, &iNeighborIndex))
	{
		return true;
	}

	/* 원래 있던 삼각형을 벗어난거야. */
	else
	{
		/* 나간쪽에 이웃이 있다라면. */
		if (-1 != iNeighborIndex)
		{
			while (true)
			{
				if (-1 == iNeighborIndex)
					return false; //임시

				if (true == m_Cells[iNeighborIndex]->isIn(vLocalPos, &iNeighborIndex))
					break;
			}


			m_iCurrentCellIndex = iNeighborIndex;
			return true;
		}

		/* 나간쪽에 이웃이 없다라면. */
		else
			return false;
	}
}


HRESULT CNavigation::Render()
{
	if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	_float4		vColor = -1 == m_iCurrentCellIndex ? _float4(0.f, 1.f, 0.f, 1.f) : _float4(1.f, 0.f, 0.f, 1.f);
	_float4x4	WorldMatrix = m_WorldMatrix;

	if (-1 != m_iCurrentCellIndex)
		WorldMatrix._42 += 0.1f;

	if (FAILED(m_pShader->Bind_Matrix("g_WorldMatrix", &WorldMatrix)))
		return E_FAIL;

	m_pShader->Bind_RawValue("g_vColor", &vColor, sizeof(_float4));

	m_pShader->Begin(0);

	if (-1 == m_iCurrentCellIndex)
	{
		for (auto& pCell : m_Cells)
			pCell->Render();
	}
	else
	{
		m_Cells[m_iCurrentCellIndex]->Render();
	}


	return S_OK;
}
void CNavigation::Rending_All()
{
	for (auto pCell : m_Cells)
		pCell->Render();
}


_float4x4 CNavigation::Get_WorldMatrix_Inverse()
{
	_matrix WorldMatrixInverse = XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_WorldMatrix));

	_float4x4 result;

	XMStoreFloat4x4(&result, WorldMatrixInverse);

	return result;
}

_fvector CNavigation::Get_Center(_int _iCellIndex)
{
	return m_Cells[_iCellIndex]->Get_Center();
}

_float CNavigation::Compute_Height(const _fvector& vLocalPos)
{
	return m_Cells[m_iCurrentCellIndex]->Compute_Height(vLocalPos);
}

_bool CNavigation::isPicking(_float3* pOut)
{
	_float3 A;
	_float3 B;
	_float3 C;
	for (auto& pCell : m_Cells)
	{
		XMStoreFloat3(&A, pCell->Get_Point(CCell::POINT_A));
		XMStoreFloat3(&B, pCell->Get_Point(CCell::POINT_B));
		XMStoreFloat3(&C, pCell->Get_Point(CCell::POINT_C));

		if (m_pGameInstance->isPicked_InWorldSpace(A, B, C, pOut))
		{

			cout << pCell->Get_Index() << endl;


			return true;
		}
	}
	return false;
}


HRESULT CNavigation::SetUp_Neighbors()
{
	for (auto& pSourCell : m_Cells)
	{
		for (auto& pDestCell : m_Cells)
		{
			if (pSourCell == pDestCell)
				continue;

			if (true == pDestCell->Compare_Points(pSourCell->Get_Point(CCell::POINT_A), pSourCell->Get_Point(CCell::POINT_B)))
			{
				pSourCell->Set_Neighbor(CCell::LINE_AB, pDestCell);
			}

			if (true == pDestCell->Compare_Points(pSourCell->Get_Point(CCell::POINT_B), pSourCell->Get_Point(CCell::POINT_C)))
			{
				pSourCell->Set_Neighbor(CCell::LINE_BC, pDestCell);
			}

			if (true == pDestCell->Compare_Points(pSourCell->Get_Point(CCell::POINT_C), pSourCell->Get_Point(CCell::POINT_A)))
			{
				pSourCell->Set_Neighbor(CCell::LINE_CA, pDestCell);
			}
		}
	}

	return S_OK;
}

HRESULT CNavigation::Ready_Cells(ifstream* _LoadStream)
{
	//로드할거
	_float3 vPoints[3];
	_uint iNumFaces(0);
	_vector transformPoints[3];
	_matrix preTransformMatrix = XMLoadFloat4x4(&m_PreTransformMatrix);

	_LoadStream->read((char*)&iNumFaces, sizeof(_uint));

	for (size_t i = 0; i < iNumFaces; i++)
	{
		_LoadStream->read((char*)&vPoints[0], sizeof(_float3));
		_LoadStream->read((char*)&vPoints[1], sizeof(_float3));
		_LoadStream->read((char*)&vPoints[2], sizeof(_float3));
		for (size_t j = 0; j < 3; j++)
		{
			transformPoints[j] = XMVector3Transform(XMLoadFloat3(&vPoints[j]), preTransformMatrix);
			XMStoreFloat3(&vPoints[j], transformPoints[j]);
		}
		CCell* pCell = CCell::Create(m_pDevice, m_pContext, vPoints, i);
		if (nullptr == pCell)
			return E_FAIL;

		m_Cells.emplace_back(pCell);
	}


	return S_OK;
}

// 교차하면 true
_bool CNavigation::CrossCheck(_float3 _vStart, _float3 _vEnd, _float3 _vP1, _float3 _vP2)
{
	_vector vStart = XMLoadFloat2(&_float2(_vStart.x, _vStart.z));
	_vector vEnd = XMLoadFloat2(&_float2(_vEnd.x, _vEnd.z));
	_vector vP1 = XMLoadFloat2(&_float2(_vP1.x, _vP1.z));
	_vector vP2 = XMLoadFloat2(&_float2(_vP2.x, _vP2.z));

	if (XMVector2Equal(vStart, vP1) || XMVector2Equal(vStart, vP2)
		|| XMVector2Equal(vEnd, vP1) || XMVector2Equal(vEnd, vP2))
		return true;

	_vector MainLine = vEnd - vStart;
	_vector Line1 = vP1 - vStart;
	_vector Line2 = vP2 - vStart;

	return 0 > XMVectorGetX(XMVector2Cross(MainLine, Line1)) * XMVectorGetX(XMVector2Cross(MainLine, Line2));
}

_float3 CNavigation::Closer(_float3 _vMainPos, _float3 _vP1, _float3 _vP2)
{
	return XMVectorGetX(XMVector2Length(XMLoadFloat2(&_float2{ _vP1.x - _vMainPos.x, _vP1.z - _vMainPos.z }))) <=
		XMVectorGetX(XMVector2Length(XMLoadFloat2(&_float2{ _vP2.x - _vMainPos.x, _vP2.z - _vMainPos.z }))) ? _vP1 : _vP2;
}

_float3 CNavigation::Brand_NEW_Closer(_float3 _vCompare1, _float3 _vCompare2, _float3 _vNextLineCenter)
{
	return XMVectorGetX(XMVector2Length(XMLoadFloat2(&_float2{ _vCompare1.x - _vNextLineCenter.x, _vCompare1.z - _vNextLineCenter.z }))) >
		XMVectorGetX(XMVector2Length(XMLoadFloat2(&_float2{ _vCompare1.x - _vNextLineCenter.x, _vCompare1.z - _vNextLineCenter.z }))) ? _vCompare1 : _vCompare2;
}

_uint CNavigation::Find_MyCell(_fvector _WorldPos)
{
	for (auto pCell : m_Cells)
	{
		if (pCell->isIn(_WorldPos, nullptr))
		{
			return pCell->Get_Index();
		}
	}


	return -1;
}


CNavigation* CNavigation::Create(ID3D11, ifstream* _LoadStream, _fmatrix PreTransformMatrix)
{
	CNavigation* pInstance = new CNavigation(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype(_LoadStream, PreTransformMatrix)))
	{
		MSG_BOX(TEXT("Failed to Created : CNavigation"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CNavigation::Clone(void* pArg)
{
	CNavigation* pInstance = new CNavigation(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CNavigation"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNavigation::Free()
{
	__super::Free();

	for (auto& pCell : m_Cells)
		Safe_Release(pCell);

	Safe_Release(m_pShader);

	m_Cells.clear();
}
