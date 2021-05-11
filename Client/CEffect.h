#pragma once
#include "CObj.h"
#include "CImage.h"

class CEffect : public CObj
{
public:
	CEffect();

	// CObj��(��) ���� ��ӵ�
	virtual void Ready() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void Release() override;

	void SetInfo(INFO& info) { m_tInfo = info; }

	LJH::CImage* m_Image;
};
