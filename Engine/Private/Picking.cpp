#include "..\Public\Picking.h"
#include "GameInstance.h"
CPicking::CPicking(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext)
	: m_pDevice{ _pDevice }
	, m_pContext{ _pContext }
	, m_pGameInstance{ CGameInstance::Get_Instance() }
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pGameInstance);
}

HRESULT CPicking::Initialize(HWND hWnd, _uint iWinSizeX, _uint iWinSizeY)
{
	m_hWnd = hWnd;

	m_iWinSizeX = iWinSizeX;

	m_iWinSizeY = iWinSizeY;

	return S_OK;
}

void CPicking::Update()
{

	GetCursorPos(&m_tMousePos);

	/* 뷰포트 == 로컬 * 월드행렬 * 뷰행렬 * 투영행렬 /w -> 뷰포트 변환 */
	ScreenToClient(m_hWnd, &m_tMousePos);

	/* 투영스페이스 == 로컬 * 월드행렬 * 뷰행렬 * 투영행렬 /w */
	_float3		vMousePos{};
	vMousePos.x = m_tMousePos.x / (m_iWinSizeX * 0.5f) - 1.f;
	vMousePos.y = m_tMousePos.y / (m_iWinSizeY * -0.5f) + 1.f;
	vMousePos.z = 0.f; /* Near평면을 클릭한거야!! */

	_vector		vMousePosvec{}, vRayPosvec{}, vRayDirvec{};

	vMousePosvec = XMLoadFloat3(&vMousePos);

	//역행렬들 준비하기
	_matrix ProjMatrixInv = m_pGameInstance->Get_Transform_Inverse_Matrix(CPipeLine::D3DTS_PROJ);
	_matrix ViewMatrixInv = m_pGameInstance->Get_Transform_Inverse_Matrix(CPipeLine::D3DTS_VIEW);

	vMousePosvec = XMVector3TransformCoord(vMousePosvec, ProjMatrixInv);


	//뷰스페이스에서 마우스포즈의 시작점과 끝점을 구함
	m_vRayPos = _float3{ 0.f, 0.f, 0.f };
	vRayPosvec = XMLoadFloat3(&m_vRayPos);

	XMStoreFloat3(&m_vRayDir, (vMousePosvec - vRayPosvec));
	vRayDirvec = XMLoadFloat3(&m_vRayDir);

	vRayPosvec = XMVector3TransformCoord(vRayPosvec, ViewMatrixInv);
	vRayDirvec = XMVector3TransformNormal(vRayDirvec, ViewMatrixInv);

	vRayDirvec = XMVector3Normalize(vRayDirvec);

	XMStoreFloat3(&m_vRayPos, vRayPosvec);
	XMStoreFloat3(&m_vRayDir, vRayDirvec);
}

void CPicking::Transform_ToLocalSpace(const _float4x4& _WorldMatrix)
{
	_matrix		WorldMatrix = XMLoadFloat4x4(&_WorldMatrix);

	_matrix		WorldMatrixInverse = XMMatrixInverse(nullptr, WorldMatrix);

	_vector rayPos = XMLoadFloat3(&m_vRayPos);
	_vector rayDir = XMLoadFloat3(&m_vRayDir);

	_vector rayPosInLocalSpace = XMVector3TransformCoord(rayPos, WorldMatrixInverse);
	_vector rayDirInLocalSpace = XMVector3TransformNormal(rayDir, WorldMatrixInverse);

	rayDirInLocalSpace = XMVector3Normalize(rayDirInLocalSpace);

	XMStoreFloat3(&m_vRayPos_InLocalSpace, rayPosInLocalSpace);
	XMStoreFloat3(&m_vRayDir_InLocalSpace, rayDirInLocalSpace);
}

_bool CPicking::isPicked_InWorldSpace(const _float3& vPointA, const _float3& vPointB, const _float3& vPointC, _float3* pOut)
{
	_vector vecA = XMLoadFloat3(&vPointA);
	_vector vecB = XMLoadFloat3(&vPointB);
	_vector vecC = XMLoadFloat3(&vPointC);
	_vector rayPos = XMLoadFloat3(&m_vRayPos);
	_vector rayDir = XMLoadFloat3(&m_vRayDir);

	float fDist = 0.0f;

	if (TriangleTests::Intersects(rayPos, rayDir, vecA, vecB, vecC, fDist))
	{
		//만약에 dist도 필요하면 여기서 꺼내오는게 좋을듯
		_vector intersection = rayPos + rayDir * fDist;
		XMStoreFloat3(pOut, intersection);
		return true;
	}

	return false;
}

_bool CPicking::isPicked_InLocalSpace(const _float3& vPointA, const _float3& vPointB, const _float3& vPointC, _float3* pOut)
{
	_vector vecA = XMLoadFloat3(&vPointA);
	_vector vecB = XMLoadFloat3(&vPointB);
	_vector vecC = XMLoadFloat3(&vPointC);
	_vector rayPos = XMLoadFloat3(&m_vRayPos_InLocalSpace);
	_vector rayDir = XMLoadFloat3(&m_vRayDir_InLocalSpace);

	float fDist = 0.0f;

	if (TriangleTests::Intersects(rayPos, rayDir, vecA, vecB, vecC, fDist))
	{
		//만약에 dist도 필요하면 여기서 꺼내오는게 좋을듯
		_vector intersection = rayPos + rayDir * fDist;
		XMStoreFloat3(pOut, intersection);
		return true;
	}

	return false;
}

CPicking* CPicking::Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext, HWND hWnd, _uint iWinSizeX, _uint iWinSizeY)
{
	CPicking* pInstance = new CPicking(_pDevice, _pContext);

	if (FAILED(pInstance->Initialize(hWnd, iWinSizeX, iWinSizeY)))
	{
		MSG_BOX(TEXT("Failed to Created : CPicking"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPicking::Free()
{
	__super::Free();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	Safe_Release(m_pGameInstance);
}
