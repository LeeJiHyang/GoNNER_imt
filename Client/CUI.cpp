#include "stdafx.h"
#include "CUI.h"
#include "CImageManager.h"
#include "CPlayer.h"

CUI::CUI()
{
	m_objID = OBJ::OBJ_UI;

	LJH::CImageManager* pImageManager = LJH::CImageManager::GetInstance();

// HP
	for (int i = 0; i < 5; i++) {
		LJH::CImage* pImage = new LJH::CImage((*pImageManager->GetImage(L"sprites", L"hp")));
		pImage->SetCam(false);
		pImage->SetCurrentFrame(rand() % pImage->GetMaxFrame());
		m_vecImageHP.emplace_back(pImage);
	}
// Back
	for (int i = 0; i < 1; i++) {
		LJH::CImage* pImage = new LJH::CImage((*pImageManager->GetImage(L"player", L"back_ammo")));
		pImage->SetCam(false);
		m_vecImageBack.emplace_back(pImage);
	}
// Bullet
	for (int i = 0; i < 20; i++) {
		LJH::CImage* pImage = new LJH::CImage((*pImageManager->GetImage(L"weapons", L"bullet1")));
		pImage->SetCam(false);
		pImage->SetCurrentFrame(rand() % pImage->GetMaxFrame());
		m_vecImageBullet.emplace_back(pImage);
	}

}

void CUI::Ready()
{
}

void CUI::Update()
{
	for (size_t i = 0; i < m_vecImageHP.size(); i++)
		m_vecImageHP[i]->Update();

	for (size_t i = 0; i < m_vecImageBack.size(); i++)
		m_vecImageBack[i]->Update();

	for (size_t i = 0; i < m_vecImageBullet.size(); i++)
		m_vecImageBullet[i]->Update();
}

void CUI::LateUpdate()
{
}

void CUI::Render()
{
	// HP
	{
		int iHP = m_pPlayer->GetHP();
		int iHPMax = m_pPlayer->GetHPMax();

		INFO info;
		info.vPos.x = 200.f;
		info.vPos.y = (float)(WINCY - 200);
		info.vScale = { 0.7f, 0.7f, 0.f };
		info.color = D3DCOLOR_ARGB(255, 164, 223, 215);
		for (int i = 0; i < iHP; ++i) {
			m_vecImageHP[i]->Render(&info);
			info.vPos.x += 30.f;
		}
		
		info.vPos.y += 20.f;
		info.color = D3DCOLOR_ARGB(255, 45, 52, 47);
		for (int i = iHP; i < iHPMax; ++i) {
			m_vecImageHP[i]->Render(&info);
			info.vPos.x += 30.f;
		}
	}

	// Back
	{
		INFO info;
		info.vPos.x = 180.f;
		info.vPos.y = (float)(WINCY - 120);
		info.color = D3DCOLOR_ARGB(255, 91, 137, 139);
		for (size_t i = 0; i < m_vecImageBack.size(); i++) {
			m_vecImageBack[i]->Render(&info);
		}
	}

	// Bullet
	{
		int iBullet = m_pPlayer->GetBullet();
		int iBulletMax = m_pPlayer->GetBulletMax();

		INFO info;
		info.vPos.x = 240.f;
		info.vPos.y = (float)(WINCY - 120);
		info.vScale = { 0.7f, 0.7f, 0.f };
		info.color = D3DCOLOR_ARGB(255, 255, 248, 82);

		for (size_t i = 0; i < iBullet; i++) {
			m_vecImageBullet[i]->Render(&info);
			info.vPos.x += 25.f;
		}

		info.vPos.y += 20.f;
		info.color = D3DCOLOR_ARGB(255, 53, 49, 22);
		for (size_t i = iBullet; i < iBulletMax; i++) {
			m_vecImageBullet[i]->Render(&info);
			info.vPos.x += 25.f;
		}

	}
}

void CUI::Release()
{
	// HP
	{
		vector<LJH::CImage*>::iterator iter = m_vecImageHP.begin();
		while (iter != m_vecImageHP.end()) {
			delete (*iter);
		}
		m_vecImageHP.clear();
	}
	// Back
	{
		vector<LJH::CImage*>::iterator iter = m_vecImageBack.begin();
		while (iter != m_vecImageBack.end()) {
			delete (*iter);
		}
		m_vecImageBack.clear();
	}
	// Bullet
	{
		vector<LJH::CImage*>::iterator iter = m_vecImageBullet.begin();
		while (iter != m_vecImageBullet.end()) {
			delete (*iter);
		}
		m_vecImageBullet.clear();
	}
}
