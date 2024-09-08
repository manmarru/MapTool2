#include "..\Public\Model.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "Animation.h"


CModel::CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent{ pDevice, pContext }
{

}

CModel::CModel(const CModel& Prototype)
	: CComponent{ Prototype }
	, m_iNumMeshes{ Prototype.m_iNumMeshes }
	, m_Meshes{ Prototype.m_Meshes }
	, m_iNumMaterials{ Prototype.m_iNumMaterials }
	, m_Materials{ Prototype.m_Materials }
	, m_PreTransformMatrix{ Prototype.m_PreTransformMatrix }
	, m_iCurrentAnimIndex{ Prototype.m_iCurrentAnimIndex }
	, m_iNumAnimations{ Prototype.m_iNumAnimations }
{

	for (auto& pPrototypeAnimation : Prototype.m_Animations)
		m_Animations.emplace_back(pPrototypeAnimation->Clone());


	for (auto& pPrototypeBone : Prototype.m_Bones)
		m_Bones.emplace_back(pPrototypeBone->Clone());

	for (auto& Material : m_Materials)
	{
		for (auto& pTexture : Material.pMaterialTextures)
			Safe_AddRef(pTexture);
	}


	for (auto& pMesh : m_Meshes)
		Safe_AddRef(pMesh);


}

_int CModel::Get_BoneIndex(const _char* pBoneName) const
{
	_uint	iBoneIndex = { 0 };
	auto	iter = find_if(m_Bones.begin(), m_Bones.end(), [&](CBone* pBone)->_bool
		{
			if (0 == strcmp(pBone->Get_Name(), pBoneName))
				return true;
			++iBoneIndex;
			return false;
		});

	if (iter == m_Bones.end())
		MSG_BOX(TEXT("���µ�?"));

	return iBoneIndex;
}

HRESULT CModel::Initialize_Prototype(TYPE eType, const _char* pModelFilePath, _fmatrix PreTransformMatrix)
{
	_uint		iFlag = { 0 };

	/* ���� : ��� �޽ð� �� ������ �������� �׎´ϴ�. */
	/* ���� : ���� �����ϴ� ��� �޽õ��� ���� ������ ����(�޽ø� ��ġ�ϱ����� ������ ��ġ�� ���缭)��� �̸� ��ȯ���ش�.*/

	iFlag = aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast;

	if (eType == TYPE_NONANIM)
		iFlag |= aiProcess_PreTransformVertices;

	m_pAIScene = m_Importer.ReadFile(pModelFilePath, iFlag);
	if (nullptr == m_pAIScene)
		return E_FAIL;

	XMStoreFloat4x4(&m_PreTransformMatrix, PreTransformMatrix);
	m_eType = eType;

	/*  ������ ������ �ε��Ѵ�. ���� ( aiNode, aiBone, aiNodeAnim ) */
	/* aiNode : �θ�, �ڽ��� �, �ڽ��� ��������? , �ʱ��������� �����ͼ� �θ�κ��� �Ļ��� ����� ���Ͽ� �������λ�������� ����� �ִ� ������ �������ش�. */
	/* aiBone : �޽ÿ� ���ԵǾ��ִ� ���� ������ �ǹ�. �� ���� � �޽��� �����鿡�� ������ �ִ���? �󸶳� ���⤷���ִ���? �󸶳� ����(offset)���ʿ�����? */
	/* aiNodeAnim(== Channel) : �ִϸ��̼ǿ��� ����ϴ� ���� ������ �ǹ�. �� �ִϸ��̼��� �����Կ� �־� � �ð��뿡 �� ���� � ���¸� ���ؾ��ϴ����� ���� ��������� �����ϱ����� ��Ḧ �����Ѵ�. */
	if (FAILED(Ready_Bones(m_pAIScene->mRootNode, -1)))
		return E_FAIL;

	if (FAILED(Ready_Meshes()))
		return E_FAIL;

	if (FAILED(Ready_Materials(pModelFilePath)))
		return E_FAIL;

	if (FAILED(Ready_Animations()))
		return E_FAIL;

	return S_OK;
}

HRESULT CModel::Initialize(void* pArg)
{

	return S_OK;
}

HRESULT CModel::Render(_uint iMeshIndex)
{
	m_Meshes[iMeshIndex]->Bind_Buffers();
	m_Meshes[iMeshIndex]->Render();

	return S_OK;
}

_bool CModel::Play_Animation(_float fTimeDelta)
{
	// �ִϸ��̼� ��ȯ���������� �ӽ�ä���� �����ߵɰŰ���
	if (m_isChanging)
	{
		Change_Animation(fTimeDelta);
	}

	/* ���� �����δ�.(CBone`s m_TransformationMatrix����� �����Ѵ�.) */
	_bool isFinished = m_Animations[m_iCurrentAnimIndex]->Update_TransformationMatrices(m_Bones, m_isLoop, fTimeDelta);

	/* ��� ���� ������ �ִ� m_CombinedTransformationMatrix�� �����Ѵ�. */
	for (auto& pBone : m_Bones)
	{
		/* �� ���� ��� * �θ��� �Ĺ��ε� */
		pBone->Update_CombinedTransformationMatrix(m_Bones, XMLoadFloat4x4(&m_PreTransformMatrix));
	}

	return isFinished;
}

void CModel::Change_Animation(_float fTimeDelta)
{
	//if (m_Animations[m_iCurrentAnimIndex]->Change_Animation(m_Bones, fTimeDelta, m_Animations[m_iNextAnimIndex]->Get_NextKeyFrame()))
	//{

	//	m_isChanging = false;
	//	m_iCurrentAnimIndex = m_iNextAnimIndex;
	//}

	for (auto& pBone : m_Bones)
	{
		pBone->Update_CombinedTransformationMatrix(m_Bones, XMLoadFloat4x4(&m_PreTransformMatrix));
	}

	return;
}

HRESULT CModel::Bind_Material(CShader* pShader, const _char* pConstantName, aiTextureType eMaterialType, _uint iMeshIndex)
{
	_uint iMaterialIndex = m_Meshes[iMeshIndex]->Get_MaterialIndex();

	return m_Materials[iMaterialIndex].pMaterialTextures[eMaterialType]->Bind_ShadeResource(pShader, pConstantName, 0);
}

HRESULT CModel::Bind_MeshBoneMatrices(CShader* pShader, const _char* pConstantName, _uint iMeshIndex)
{
	m_Meshes[iMeshIndex]->Bind_BoneMatrices(this, pShader, pConstantName);

	return S_OK;
}

HRESULT CModel::Ready_Meshes()
{
	m_iNumMeshes = m_pAIScene->mNumMeshes;

	for (size_t i = 0; i < m_iNumMeshes; i++)
	{
		CMesh* pMesh = CMesh::Create(m_pDevice, m_pContext, this, m_pAIScene->mMeshes[i], XMLoadFloat4x4(&m_PreTransformMatrix));
		if (nullptr == pMesh)
			return E_FAIL;

		m_Meshes.emplace_back(pMesh);
	}

	return S_OK;
}

HRESULT CModel::Ready_Materials(const _char* pModelFilePath)
{
	m_iNumMaterials = m_pAIScene->mNumMaterials;

	for (size_t i = 0; i < m_iNumMaterials; i++)
	{
		MESH_MATERIAL		MeshMaterial{};

		aiMaterial* pAIMaterial = m_pAIScene->mMaterials[i];

		for (size_t j = 1; j < AI_TEXTURE_TYPE_MAX; j++)
		{
			// pAIMaterial->GetTextureCount(j);
			aiString			strTexturePath;

			_char				szDrive[MAX_PATH] = "";
			_char				szDirectory[MAX_PATH] = "";
			_char				szFileName[MAX_PATH] = "";
			_char				szExt[MAX_PATH] = "";

			_char				szTexturePath[MAX_PATH] = "";

			if (FAILED(pAIMaterial->GetTexture(aiTextureType(j), 0, &strTexturePath)))
				continue;

			_splitpath_s(pModelFilePath, szDrive, MAX_PATH, szDirectory, MAX_PATH, nullptr, 0, nullptr, 0);
			_splitpath_s(strTexturePath.data, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szExt, MAX_PATH);

			strcpy_s(szTexturePath, szDrive);
			strcat_s(szTexturePath, szDirectory);
			strcat_s(szTexturePath, szFileName);
			strcat_s(szTexturePath, szExt);

			_tchar				szFinalPath[MAX_PATH] = TEXT("");

			MultiByteToWideChar(CP_ACP, 0, szTexturePath, strlen(szTexturePath), szFinalPath, MAX_PATH);

			MeshMaterial.pMaterialTextures[j] = CTexture::Create(m_pDevice, m_pContext, szFinalPath, 1);
			if (nullptr == MeshMaterial.pMaterialTextures[j])
				return E_FAIL;
		}

		m_Materials.emplace_back(MeshMaterial);
	}

	return S_OK;
}

HRESULT CModel::Ready_Bones(const aiNode* pAIBone, _int iParentBoneIndex)
{
	/* ���� ������ �����Ѵ�. ���� �ؾ��� ��(�ڽĻ��� ���¸� ��ȯ�Ѵ�. * �θ���� ����� ���Ͽ� ������������� �����.)*/
	CBone* pBone = CBone::Create(pAIBone, iParentBoneIndex);
	if (nullptr == pBone)
		return E_FAIL;

	m_Bones.emplace_back(pBone);

	_int		iParentIndex = m_Bones.size() - 1;

	for (size_t i = 0; i < pAIBone->mNumChildren; i++)
	{
		Ready_Bones(pAIBone->mChildren[i], iParentIndex);
	}

	return S_OK;
}

HRESULT CModel::Ready_Animations()
{
	m_iNumAnimations = m_pAIScene->mNumAnimations;

	for (size_t i = 0; i < m_iNumAnimations; i++)
	{
		CAnimation* pAnimation = CAnimation::Create(m_pAIScene->mAnimations[i], this);
		if (nullptr == pAnimation)
			return E_FAIL;

		m_Animations.emplace_back(pAnimation);
	}
	return S_OK;
}


CModel* CModel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, TYPE eType, const _char* pModelFilePath, _fmatrix PreTransformMatrix)
{
	CModel* pInstance = new CModel(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eType, pModelFilePath, PreTransformMatrix)))
	{
		MSG_BOX(TEXT("Failed to Created : CModel"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CModel::Clone(void* pArg)
{
	CModel* pInstance = new CModel(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created : CModel"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CModel::Free()
{
	__super::Free();

	for (auto& pAnimation : m_Animations)
		Safe_Release(pAnimation);
	m_Animations.clear();

	for (auto& pBone : m_Bones)
		Safe_Release(pBone);
	m_Bones.clear();

	for (auto& Material : m_Materials)
	{
		for (auto& pTexture : Material.pMaterialTextures)
			Safe_Release(pTexture);
	}
	m_Materials.clear();

	for (auto& pMesh : m_Meshes)
	{
		Safe_Release(pMesh);
	}
	m_Meshes.clear();
}
