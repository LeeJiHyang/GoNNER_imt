#include "stdafx.h"
#include "CBubble.h"
#include "CImageManager.h"

CBubble::CBubble(INFO& infoStart, INFO& infoGoal)
{
	LJH::CImageManager* pImageManager = LJH::CImageManager::GetInstance();

	// Image
	LJH::CImage* pImage = new LJH::CImage(*pImageManager->GetImage(L"bigsprites", L"circleparticle"));
	pImage->SetPS(ANI_LOOP);
	m_Image = pImage;

	infoStart.vPos.x += (float)((rand() % 200) - 100);
	infoStart.vPos.y += (float)((rand() % 200) - 100);

	m_infoDraw = infoStart;
	m_infoGoal = infoGoal;
	m_tInfo = infoStart;

	// Time
	m_fStartTime = TIMEMANAGER->GetWorldTime();
	m_fEndTime = ((rand() % 20) + 30) * 0.1f; // 3.0 ~ 5.0
	
	m_objID = OBJ::OBJ_BUBBLE;
}

void CBubble::Ready()
{
}

void CBubble::Update()
{
	LJH::CImageManager* pImageManager = LJH::CImageManager::GetInstance();

	if (!m_bDeadStart) {
		float fCurrTime = TIMEMANAGER->GetWorldTime();
		if ((fCurrTime - m_fStartTime) > m_fEndTime) {
			SetDeadStart(true);
			SafeDelete(m_Image);
			m_Image = new LJH::CImage(*pImageManager->GetImage(L"bigsprites", L"circleparticleend"));
			m_Image->SetPS(ANI_NOLOOP);
			return;
		}
	}

	m_Image->Update();

	if (m_bDeadStart) return;

	// 방향 벡터
	D3DXVec3Lerp(&m_infoDraw.vDir, &m_infoDraw.vDir, &m_infoGoal.vDir, 0.1f);

	// 방향 벡터 위치 적용
	m_tInfo.vDir = m_infoDraw.vDir;
	m_tInfo.vPos += m_tInfo.vDir * g_ETime;

	// 크기
	D3DXVec3Lerp(&m_infoDraw.vScale, &m_infoDraw.vScale, &m_infoGoal.vScale, 0.01f);
	m_tInfo.vScale = m_infoDraw.vScale;

}

void CBubble::LateUpdate()
{
}

void CBubble::Render()
{
	if (m_Image)
		m_Image->Render(&m_tInfo);
}

void CBubble::Release()
{
	SafeDelete(m_Image);
}
