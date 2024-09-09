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
    CNavigation(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext);
    CNavigation(const CNavigation& Prototype);
    virtual ~CNavigation() = default;

public:
    virtual HRESULT Initialize_Prototype(const _char* pModelFilePath, _fmatrix PreTransformMatrix);
    virtual HRESULT Initialize(void* pArg) override;
    void Update(_fmatrix TerrainWorldMatrix);
#ifdef _DEBUG
    HRESULT Render();
#endif // _DEBUG


private:
    vector<class CCell*>    m_Cells;
	_int                    m_iCurrentCellIndex = { -1 };
    static _float4x4        m_WorldMatrix;

#ifdef _DEBUG
private:
    class CShader* m_pShader = { nullptr };
#endif

private:
    _float4x4                   m_PreTransformMatrix = {};
    Assimp::Importer            m_Importer;
    const aiScene*              m_pAIScene = { nullptr };
    const aiMesh*               m_pAIMesh = { nullptr };
    _uint                       m_iNumMeshes = { 0 };
    _uint                       m_iNumVertices = { 0 };
    _uint                       m_iNumFaces = { 0 };
    _uint                       m_iIndexStride = 4;
    _uint						m_iVertexStride = {};
    DXGI_FORMAT					m_eIndexFormat = {};
    D3D11_PRIMITIVE_TOPOLOGY	m_eTopology = {};

private:
    HRESULT SetUp_Neighbors();

public:
    HRESULT Ready_Cells();

public:
    static CNavigation* Create(ID3D11Device* _pDevice, ID3D11DeviceContext* _pContext, const _char* pModelFilePath, _fmatrix PreTransformMatrix);
    virtual CComponent* Clone(void* pArg) override;
    virtual void Free() override;
};

END