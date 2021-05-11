#include "stdafx.h"
#include "CObj.h"

float CObj::GetX()
{
	return m_tInfo.vPos.x;
}

float CObj::GetY()
{
	return m_tInfo.vPos.y;
}

void CObj::SetX(float fX)
{
	m_tInfo.vPos.x = fX;
}

void CObj::SetY(float fY)
{
	m_tInfo.vPos.y = fY;
}

void CObj::SetPos(float fX, float fY)
{
	m_tInfo.vPos.x = fX;
	m_tInfo.vPos.y = fY;
}

int CObj::GetCX()
{
	return (int)m_tInfo.vSize.x;
}

int CObj::GetCY()
{
	return (int)m_tInfo.vSize.y;
}

float CObj::GetSpeed()
{
	return m_fSpeed;
}

void CObj::SetSpeed(float fSpeed)
{
	m_fSpeed = fSpeed;
}

RECT CObj::GetCol()
{
	return { LONG(GetX() - (GetCX() >> 1)),
			 LONG(GetY() - (GetCY() >> 1)),
			 LONG(GetX() + (GetCX() >> 1)),
			 LONG(GetY() + (GetCY() >> 1)) };
}

void CObj::ActiveGravity()
{
	if (!m_bGravity) return;

	// 중력작용
	m_tInfo.vPos.y -= sinf(D3DXToRadian(m_fAngle)) * m_fForce;
	m_tInfo.vPos.y += m_fGravity;
	m_fGravity += GRAVITY_VALUE;
}

void CObj::DeactivateGravity()
{
	m_fGravity = 0.f;
}

void CObj::SetDeadStart(bool bDeadStart)
{
	m_bDeadStart = bDeadStart;
	m_fDeadStartTime = TIMEMANAGER->GetWorldTime();
}

void CObj::SetDead(bool bDead)
{
	m_bDead = bDead;
}
