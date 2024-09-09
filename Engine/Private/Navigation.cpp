#include "Navigation.h"
#include "Cell.h"

#include "Shader.h"
#include "GameInstance.h"

_float4x4 CNavigation::m_WorldMatrix = {};

CNavigation::CNavigation(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	:CComponent{ _pDevice, _pContext }
{
}

CNavigation::CNavigation(const CNavigation& Prototype)
	:CComponent(Prototype)
	, m_Cells{ Prototype.m_Cells }
#ifdef _DEBUG
	, m_pShader{ Prototype.m_pShader }
#endif
{
	for (auto& pCell : m_Cells)
		Safe_AddRef(pCell);
#ifdef _DEBUG
	Safe_AddRef(m_pShader);
#endif
}

HRESULT CNavigation::Initialize_Prototype(const _char* pModelFilePath, _fmatrix PreTransformMatrix)
{
	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());

	_uint iFlag = { 0 };

	iFlag = aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast | aiProcess_PreTransformVertices;

	m_pAIScene = m_Importer.ReadFile(pModelFilePath, iFlag);
	if (nullptr == m_pAIScene)
		return E_FAIL;

	XMStoreFloat4x4(&m_PreTransformMatrix, PreTransformMatrix);

	if (FAILED(Ready_Cells()))
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
	return S_OK;
}

void CNavigation::Update(_fmatrix TerrainWorldMatrix)
{
	XMStoreFloat4x4(&m_WorldMatrix, TerrainWorldMatrix);
}

#ifdef _DEBUG

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

#endif

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

HRESULT CNavigation::Ready_Cells()
{
	//일단은 무조건 메쉬가 하나라고 가정할거임
	m_pAIMesh = m_pAIScene->mMeshes[0];
	
	//face에서 점 세개 꺼내서 CCell생성하고 face넘버 인덱스넘버로 넘긴다.
	_float3 vPoints[3];
	_uint vIndices[3];

	for (size_t i = 0; i < m_pAIMesh->mNumFaces; i++)
	{
		vIndices[0] = m_pAIMesh->mFaces[i].mIndices[0];
		vIndices[1] = m_pAIMesh->mFaces[i].mIndices[1];
		vIndices[2] = m_pAIMesh->mFaces[i].mIndices[2];

		memcpy(&vPoints[0], &_float3(m_pAIMesh->mVertices[vIndices[0]].x * 30.f, m_pAIMesh->mVertices[vIndices[0]].y* 30.f, m_pAIMesh->mVertices[vIndices[0]].z* 30.f), sizeof(_float3));
		memcpy(&vPoints[1], &_float3(m_pAIMesh->mVertices[vIndices[1]].x * 30.f, m_pAIMesh->mVertices[vIndices[1]].y* 30.f, m_pAIMesh->mVertices[vIndices[1]].z* 30.f), sizeof(_float3));
		memcpy(&vPoints[2], &_float3(m_pAIMesh->mVertices[vIndices[2]].x * 30.f, m_pAIMesh->mVertices[vIndices[2]].y* 30.f, m_pAIMesh->mVertices[vIndices[2]].z* 30.f), sizeof(_float3));

		CCell* pCell = CCell::Create(m_pDevice, m_pContext, vPoints, i);
		if (nullptr == pCell)
			return E_FAIL;

		m_Cells.emplace_back(pCell);
	}


	return S_OK;
}

CNavigation* CNavigation::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext, const _char* pModelFilePath, _fmatrix PreTransformMatrix)
{
	CNavigation* pInstance = new CNavigation(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize_Prototype(pModelFilePath, PreTransformMatrix)))
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

	m_Cells.clear();
}
