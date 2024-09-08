#pragma once

#include "Base.h"

BEGIN(Engine)

/* �ִԿ��ó��� ������ ���� ����ϴ� �� �� �ϳ� */
/* �ð��� ���� ���� ������ ������ �ִ´�. �ʱ� ������ ��� �ð��� ���� ���°����� �̸� �����صд�. */
/* ���� ��Ÿ�Ӱ��� �� ä�ο��� ���� �ִϸ��̼��� ��� ��ġ�� �������ָ� �����ġ�� �´� �������(m_TransformatoinMatrix )�� �����. */

/* KEYFRAME : Ư�� �����ġ���� �� ���� ���ؾ��� ����(���)�� ����(������)�Ͽ� ǥ���� ����ü.*/

class CChannel final : public CBase
{
private:
	CChannel();
	virtual ~CChannel() = default;

public:
	HRESULT Initialize(const aiNodeAnim* pAIChannel, const class CModel* pModel);
	void Update_TransformationMatrix(const vector<class CBone*>& Bones, _uint* pCurrentKeyFrameIndex, _double CurrentTrackPosition);

public:
	KEYFRAME Get_FirstKEYFRAME() { return m_KeyFrames.front(); }
	KEYFRAME Get_LastKEYFRAME() { return m_KeyFrames.back(); }

private:
	_char				m_szName[MAX_PATH] = {};

	_uint				m_iBoneIndex = { 0 };

	_uint				m_iNumKeyFrames = {};
	vector<KEYFRAME>	m_KeyFrames;

public:
	static CChannel* Create(const aiNodeAnim* pAIChannel, const class CModel* pModel);
	virtual void Free() override;
};

END