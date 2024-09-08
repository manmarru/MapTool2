#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Terrain final : public CVIBuffer
{
private:
	CVIBuffer_Terrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Terrain(const CVIBuffer_Terrain& Prototype);
	virtual ~CVIBuffer_Terrain() = default;

public:
	virtual HRESULT Initialize_Prototype(int iNumVerticesX, int iNumVerticesZ);
	virtual HRESULT Initialize(void* pArg) override;

public:
	void Resize(int iSizeX, int iSizeZ);
	virtual _bool isPicking(const _float4x4& WorldMatrix, _float3* pOut) override;

private:
	_uint			m_iNumVerticesX = {};
	_uint			m_iNumVerticesZ = {};

public:
	static CVIBuffer_Terrain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _int iX, _int iZ);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};


END