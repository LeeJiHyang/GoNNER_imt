#include "stdafx.h"
#include "CCameraManager.h"

void CCameraManager::Update()
{
}

void CCameraManager::LateUpdate()
{
	vGoalCam.x = (float)g_ptCam.x;
	vGoalCam.y = (float)g_ptCam.y;

	D3DXVec3Lerp(&vShowCam, &vShowCam, &vGoalCam, 0.1f);
	
	m_ptCam.x = (LONG)vShowCam.x;
	m_ptCam.y = (LONG)vShowCam.y;

// Shake
	if ((TIMEMANAGER->GetWorldTime() * 1000) >= m_iTimeStartShake + m_iTimeShake) {
		m_bShake = false;
	}

	if (m_bShake) {
		int iXRand = (rand() % 10) - 5;
		int iYRand = (rand() % 10) - 5;
		m_ptCam.x += iXRand;
		m_ptCam.y += iYRand;
	}
}

void CCameraManager::Shake(int iTime)
{
	m_bShake = true;
	m_iTimeStartShake = (int)(TIMEMANAGER->GetWorldTime() * 1000);
	m_iTimeShake = iTime;
}
