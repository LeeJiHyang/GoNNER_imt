#pragma once
#include "CObj.h"
#include "CImage.h"

class CBullet :
	public CObj
{
	LJH::CImage* m_ImageBullet[1];
	int m_iBulletID = 0;

public:
	CBullet();
	CBullet(_vec3 vPos, float fAngle, int iBulletID);
	virtual ~CBullet();

	// CObj을(를) 통해 상속됨
	virtual void Ready() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void Release() override;
};

