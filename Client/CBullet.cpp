#include "stdafx.h"
#include "CBullet.h"
#include "CImageManager.h"

CBullet::CBullet()
{
	m_tInfo.color = D3DCOLOR_ARGB(255, 255, 252, 80);
	m_fSpeed = 2500.f;

	LJH::CImageManager* pImageManager = LJH::CImageManager::GetInstance();
	m_ImageBullet[0] = pImageManager->GetImage(L"weapons", L"effect1");

	m_objID = OBJ::OBJ_BULLET;
}

CBullet::CBullet(_vec3 vPos, float fAngle, int iBulletID)
{
	m_tInfo.vPos = vPos;
	m_tInfo.vScale.x = 0.5f;
	m_tInfo.vScale.y = 0.5f;
	m_tInfo.vSize.x = 30.f;
	m_tInfo.vSize.y = 20.f;
	m_tInfo.vRot.z = fAngle + 90.f;
	m_tInfo.color = D3DCOLOR_ARGB(255, 255, 252, 80);
	m_fAngle = fAngle;
	m_fSpeed = 2500.f;
	m_iBulletID = 0;

	m_tInfo.vDir.x = cosf(D3DXToRadian(m_fAngle)) * m_fSpeed;
	m_tInfo.vDir.y = sinf(D3DXToRadian(m_fAngle)) * m_fSpeed;

	LJH::CImageManager* pImageManager = LJH::CImageManager::GetInstance();
	m_ImageBullet[m_iBulletID] = pImageManager->GetImage(L"weapons", L"effect1");

	m_objID = OBJ::OBJ_BULLET;
}

CBullet::~CBullet()
{
}

void CBullet::Ready()
{
}

void CBullet::Update()
{
	if (m_bDeadStart) {
		m_bDead = true;
		return;
	}

	m_tInfo.vPos.x += m_tInfo.vDir.x * g_ETime;
	m_tInfo.vPos.y += m_tInfo.vDir.y * g_ETime;
}

void CBullet::LateUpdate()
{
}

void CBullet::Render()
{
	m_ImageBullet[m_iBulletID]->Render(&m_tInfo);
}

void CBullet::Release()
{
}
