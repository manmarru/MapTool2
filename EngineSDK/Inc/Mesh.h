#pragma once

#include "VIBuffer.h"
#include "Model.h"

/* �� = �޽� + �޽� + �޽� .. */
/* �޽÷� �����ϴ� ���� : ������ ��ü�� �����ϰ� ������ֱ� ���ؼ�.*/
/* �޽� = �������� + �ε��� */

BEGIN(Engine)

class CMesh final : public CVIBuffer
{
private:
	CMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMesh(const CMesh& Prototype);
	virtual ~CMesh() = default;	

public:
	_uint Get_MaterialIndex() const {
		return m_iMaterialIndex;
	}

public:
	virtual HRESULT Initialize_Prototype(const CModel* pModel, const aiMesh* pAIMesh, _fmatrix PreTransformMatrix);
	virtual HRESULT Initialize(void* pArg) override;

public:
	HRESULT Bind_BoneMatrices(const CModel* pModel, class CShader * pShader, const _char * pConstantName);

private:
	_char				m_szName[MAX_PATH] = {};
	_uint				m_iMaterialIndex = { 0 };
	_uint				m_iNumBones = { 0 };

	/* uint : �𵨿� �ε��س��� ��ü �� ���� �ε����� �̾߱��Ѵ�. */
	vector<_int>		m_BoneIndices;	

	_float4x4			m_BoneMatrices[g_iMaxMeshBones] = {};
	vector<_float4x4>	m_OffsetMatrices;


private:
	HRESULT	Ready_VertexBuffer_NonAnim(const aiMesh* pAIMesh, _fmatrix PreTransformMatrix);
	HRESULT	Ready_VertexBuffer_Anim(const CModel* pModel, const aiMesh* pAIMesh);


public:
	static CMesh* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const CModel* pModel, const aiMesh* pAIMesh, _fmatrix PreTransformMatrix);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END