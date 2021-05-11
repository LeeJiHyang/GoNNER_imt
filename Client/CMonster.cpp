#include "stdafx.h"
#include "CMonster.h"
#include "CImageManager.h"
#include "CCameraManager.h"
#include "CEffect.h"

wstring strMonsterImageKey[2][MS_NUM] =
{
	{ L"enemy00_idle", L"enemy00_move", L"enemy00_hit" },
	{ L"enemy01_idle", L"enemy01_move", L"enemy01_hit" }
};

CMonster::CMonster(_vec3 vPos, int iMobID)
{
	m_tInfo.vPos = vPos;
	m_tInfo.vDir = { 0.f, 0.f, 0.f };
	m_tInfo.vSize = { 30.f, 60.f, 0.f };
	m_tInfo.color = D3DCOLOR_ARGB(255, 253, 83, 78);
	m_fSpeed = 200.f;
	m_iMobID = iMobID;

	LJH::CImageManager* pImgMgr = LJH::CImageManager::GetInstance();

	for (int i = 0; i < MS_NUM; i++) {
		LJH::CImage* pImage = pImgMgr->GetImage(L"smallenemies", strMonsterImageKey[iMobID][i]);
		m_Image[i] = new LJH::CImage(*pImage);
		m_Image[i]->SetTime(0.05f);
	}

	if (iMobID == 0) {
		m_bGravity = true;
	}

	m_objID = OBJ::OBJ_MONSTER;
}

CMonster::~CMonster()
{
	
}

void CMonster::Ready()
{

}

void CMonster::Update()
{
	if (m_bDeadStart) {
		if (!m_bDrawDeadEffect) {
			DrawDeadEffect();
			CCameraManager::GetInstance()->Shake(300);
			SOUNDMANAGER->play("die");
		}
		m_bDead = true;
		m_ms = MS_HIT;
		return;
	}

// Pattern
	float fCurrTime = TIMEMANAGER->GetWorldTime();

	if (!m_bPatternOn) {
		if ((fCurrTime - m_fLastMove) > m_fNextMove) {
			m_fLastMove = fCurrTime;
			switch (m_mp)
			{
			case MP_NONE:
				m_md = (MONSTER_DIRECTION)(rand() % 2);
				m_ms = MS_MOVE;
				if (m_md == MD_LEFT) {
					m_tInfo.vDir.x -= m_fSpeed;
				}
				else {
					m_tInfo.vDir.x += m_fSpeed;
				}
				break;
			}
			m_bPatternOn = true;
		}
	}
	else {
		if ((fCurrTime - m_fLastMove) > m_fActionTime) {
			m_bPatternOn = false;
			m_ms = MS_IDLE;
			m_tInfo.vDir = { 0.f, 0.f, 0.f };
			m_fNextMove = (float)((rand() % 2) + 2);
		}
	}

	m_tInfo.vPos += m_tInfo.vDir * g_ETime;

// Gravity
	ActiveGravity();

// Update Image
	m_Image[m_ms]->Update();
}

void CMonster::LateUpdate()
{
	// Flip
	if (m_md == MD_RIGHT) m_Image[m_ms]->SetFlipY(false);
	else if (m_md == MD_LEFT) m_Image[m_ms]->SetFlipY(true);
}

void CMonster::Render()
{
	m_Image[m_ms]->Render(&m_tInfo);
}

void CMonster::Release()
{
	for (int i = 0; i < MS_NUM; i++) {
		SafeDelete(m_Image[i]);
	}
}

void CMonster::SetEffectList(list<CObj*>* pEffectList)
{
	m_pEffectList = pEffectList;
}

void CMonster::DrawDeadEffect()
{
	LJH::CImageManager* pImageManager = LJH::CImageManager::GetInstance();

	// 피격
	{
		// Info
		INFO info;
		info.vPos = m_tInfo.vPos;
		info.color = D3DCOLOR_ARGB(255, 253, 101, 82);

		// Image
		LJH::CImage* pImage = new LJH::CImage(*pImageManager->GetImage(L"bloodParticles", L"monsterhitshoteff"));
		pImage->SetPS(ANI_NOLOOP);

		// Effect
		CEffect* pEffect = new CEffect();
		pEffect->m_Image = pImage;
		pEffect->SetInfo(info);

		m_pEffectList->emplace_back(pEffect);
	}

	// 해골
	{
		// Info
		INFO info;
		info.vPos = m_tInfo.vPos;
		info.color = D3DCOLOR_ARGB(255, 253, 101, 82);

		// Image
		LJH::CImage* pImage = new LJH::CImage(*pImageManager->GetImage(L"bloodParticles", L"monsterhitskulleff"));
		pImage->SetPS(ANI_NOLOOP);

		// Effect
		CEffect* pEffect = new CEffect();
		pEffect->m_Image = pImage;
		pEffect->SetInfo(info);

		m_pEffectList->emplace_back(pEffect);
	}

	// 펼쳐지는것
	{
		// Info
		INFO info;
		info.vPos = m_tInfo.vPos;
		float fAngle = 0.f;
		float fSpeed = 500.f;

		for (int i = 0; i < 5; i++) {
			fAngle = (float)(rand() % 360);

			info.vDir.x = (cosf(D3DXToRadian(fAngle)) * fSpeed);
			info.vDir.y = (sinf(D3DXToRadian(fAngle)) * fSpeed);
			info.vRot.z = fAngle + 90.f;
			info.color = D3DCOLOR_ARGB(255, 181, 56, 61);

			// Image
			LJH::CImage* pImage = new LJH::CImage(*pImageManager->GetImage(L"bloodParticles", L"monsterhitextend1eff"));
			pImage->SetPS(ANI_NOLOOP);

			// Effect
			CEffect* pEffect = new CEffect();
			pEffect->m_Image = pImage;
			pEffect->SetInfo(info);

			m_pEffectList->emplace_back(pEffect);
		}

		for (int i = 0; i < 10; i++) {
			fAngle = (float)(rand() % 360);

			info.vDir.x = (cosf(D3DXToRadian(fAngle)) * fSpeed);
			info.vDir.y = (sinf(D3DXToRadian(fAngle)) * fSpeed);
			info.vRot.z = fAngle + 90.f;
			info.color = D3DCOLOR_ARGB(255, 155, 50, 55);

			// Image
			LJH::CImage* pImage = new LJH::CImage(*pImageManager->GetImage(L"bloodParticles", L"monsterhitextend2eff"));
			pImage->SetPS(ANI_NOLOOP);

			// Effect
			CEffect* pEffect = new CEffect();
			pEffect->m_Image = pImage;
			pEffect->SetInfo(info);

			m_pEffectList->emplace_back(pEffect);
		}
	}

	// 방울

	BUBBLEMANAGER->AddBubble(10, m_tInfo, 0.f, D3DCOLOR_ARGB(255, 80, 31, 37));

	m_bDrawDeadEffect = true;
}
