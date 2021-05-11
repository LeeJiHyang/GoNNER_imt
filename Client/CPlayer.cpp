#include "stdafx.h"
#include "CPlayer.h"
#include "CBullet.h"
#include "CImage.h"
#include "CImageManager.h"
#include "GraphicDevice.h"
#include "CTextureManager.h"
#include "CKeyManager.h"
#include "CCameraManager.h"
#include "CEffect.h"

CPlayer::CPlayer()
{
	LJH::CImageManager* pImageManager = LJH::CImageManager::GetInstance();

	m_pImageIdle = pImageManager->GetImage(L"player", L"idle");
	m_pImageWalk = pImageManager->GetImage(L"player", L"walk");
	m_pCurrentImage = m_pImageIdle;

	m_pImageHead = pImageManager->GetImage(L"player", L"head_skull");
	m_pImageBack = pImageManager->GetImage(L"player", L"back_ammo");
	m_pImageWeapon = pImageManager->GetImage(L"weapons", L"rifle");

	m_fAngle = 90.f;

	m_bGravity = true;

	m_objID = OBJ::OBJ_PLAYER;
}


CPlayer::~CPlayer()
{
}

void CPlayer::Ready()
{
	// Player
	m_tInfo.vPos = { WINCX * 0.5f, WINCY * 0.5f, 0.f };
	m_tInfo.vDir = {1.f, 1.f, 0.f}; 
	m_tInfo.vScale = {1.f, 1.f, 0.f}; 
	m_tInfo.vSize = {30.f, 60.f, 0.f}; 
	m_tInfo.vLook = {1.f, 0.f, 0.f}; 
	m_tInfo.vRot = { 0.f, 0.f, 0.f };
	m_tInfo.vRev = { 0.f, 0.f, 0.f };
	D3DXMatrixIdentity(&m_tInfo.matWorld);
	D3DXMatrixIdentity(&m_tInfo.matParent);
	m_tInfo.color = D3DCOLOR_ARGB(255, 80, 193, 156);
	m_fSpeed = 400.f;

	// Back
	m_tInfoBack.vPos = { -10.f, 10.f, 0.f };
	m_tInfoBack.vDir = { 1.f, 1.f, 0.f };
	m_tInfoBack.vScale = { 1.f, 1.f, 0.f };
	m_tInfoBack.vSize = { 1.f, 1.f, 0.f };
	m_tInfoBack.vLook = { 1.f, 0.f, 0.f };
	m_tInfoBack.vRot = { 0.f, 0.f, 0.f };
	m_tInfoBack.vRev = { 0.f, 0.f, 0.f };
	D3DXMatrixIdentity(&m_tInfoBack.matWorld);
	D3DXMatrixIdentity(&m_tInfoBack.matParent);
	m_tInfoBack.color = D3DCOLOR_ARGB(255, 96, 138, 143);

	// Weapon
	m_tInfoWeapon.vPos = { 5.f, 10.f, 0.f };
	m_tInfoWeapon.vDir = { 1.f, 1.f, 0.f };
	m_tInfoWeapon.vScale = { 1.f, 1.f, 0.f };
	m_tInfoWeapon.vSize = { 1.f, 1.f, 0.f };
	m_tInfoWeapon.vLook = { 1.f, 0.f, 0.f };
	m_tInfoWeapon.vRot = { 0.f, 0.f, 85.f };
	m_tInfoWeapon.vRev = { 0.f, 0.f, 0.f };
	D3DXMatrixIdentity(&m_tInfoWeapon.matWorld);
	D3DXMatrixIdentity(&m_tInfoWeapon.matParent);
	m_tInfoWeapon.color = D3DCOLOR_ARGB(255, 121, 191, 168);

	// Head
	m_tInfoHead.vPos = { 5.f, -10.f, 0.f };
	m_tInfoHead.vDir = { 1.f, 1.f, 0.f };
	m_tInfoHead.vScale = { 1.f, 1.f, 0.f };
	m_tInfoHead.vSize = { 1.f, 1.f, 0.f };
	m_tInfoHead.vLook = { 1.f, 0.f, 0.f };
	m_tInfoHead.vRot = { 0.f, 0.f, 0.f };
	m_tInfoHead.vRev = { 0.f, 0.f, 0.f };
	D3DXMatrixIdentity(&m_tInfoHead.matWorld);
	D3DXMatrixIdentity(&m_tInfoHead.matParent);
	m_tInfoHead.color = D3DCOLOR_ARGB(255, 166, 228, 212);
}

void CPlayer::Update()
{
	CKeyManager* pKeyManager = CKeyManager::GetInstance();

	PLAYER_STATE ps = PS_IDLE;

// Player Move
	if (GetAsyncKeyState(VK_LEFT)) {
		m_tInfo.vPos.x -= m_fSpeed * g_ETime;
		m_pd = PD_LEFT;
		ps = PS_WALK;
	}
	if (GetAsyncKeyState(VK_RIGHT)) {
		m_tInfo.vPos.x += m_fSpeed * g_ETime;
		m_pd = PD_RIGHT;
		ps = PS_WALK;
	}

	if (pKeyManager->isKeyDown(VK_SPACE)) // 점프
	{
		if (m_iJumpCount == 2) {
			
		}
		else if (m_iJumpCount == 1) {
			m_iJumpCount++;
			m_fGravity = 0.f;
			SOUNDMANAGER->play("jump");
		}
		else if (m_iJumpCount == 0) {
			m_iJumpCount++;
			m_bJump = true;
			m_fGravity = 0.f;
			SOUNDMANAGER->play("jump");
		}

	}
	if (m_bJump) {
		ps = PS_JUMP;
	}
	if (pKeyManager->StayKeyDown('X')) // 공격
	{
		Attack();
	}
	if (pKeyManager->isKeyDown('C'))
	{
		ChargeBullet();
	}

// Player State / Image
	BOOL bInitPs = m_ps != ps;
	m_ps = ps;
	     if (m_ps == PS_IDLE) m_pCurrentImage = m_pImageIdle;
	else if (m_ps == PS_WALK) m_pCurrentImage = m_pImageWalk;
	if(bInitPs) m_pCurrentImage->InitFrame();

	// Gravity
	ActiveGravity();

// Update Image
	m_pCurrentImage->Update();
	m_pImageBack->Update();
	m_pImageWeapon->Update();
	m_pImageHead->Update();

// Balloon
	auto iterBalloon = m_listBalloon.begin();
	while (iterBalloon != m_listBalloon.end()) {
		if ((*iterBalloon)->GetDead()) {
			iterBalloon++;
			continue;
		}
		(*iterBalloon)->Update();
		++iterBalloon;
	}

}

void CPlayer::LateUpdate()
{
// Flip
	if (m_pd == PD_RIGHT) m_pCurrentImage->SetFlipY(false);
	else if (m_pd == PD_LEFT) m_pCurrentImage->SetFlipY(true);

// Cam
	int iCamGap = TILEGAP * 2;

	int iCamX = (int)m_tInfo.vPos.x - (WINCX >> 1); // 캐릭터의 X 위치
	int iCamY = (int)m_tInfo.vPos.y - (WINCY >> 1); // 캐릭터의 Y 위치
	int iMaxCamX = m_pMap->GetCX() - WINCX;
	int iMaxCamY = m_pMap->GetCY() - WINCY;

	if (iCamX <          - iCamGap) iCamX =          - iCamGap;
	if (iCamX > iMaxCamX + iCamGap) iCamX = iMaxCamX + iCamGap;
	if (iCamY <          - iCamGap) iCamY =          - iCamGap;
	if (iCamY > iMaxCamY + iCamGap) iCamY = iMaxCamY + iCamGap;

	g_ptCam.x = (LONG)iCamX;
	g_ptCam.y = (LONG)iCamY;

// Matrix
	POINT ptCam = CCameraManager::GetInstance()->GetCam();
	_matrix matScale, matRotZ, matTrans;
	if(m_pd == PD_RIGHT) D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	else                 D3DXMatrixScaling(&matScale, -1.f, 1.f, 0.f);
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(m_tInfo.vRot.z));
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - ptCam.x, m_tInfo.vPos.y - ptCam.y, m_tInfo.vPos.z);
	m_tInfo.matWorld = matScale * matRotZ * matTrans;

	m_tInfoBack.matParent = m_tInfo.matWorld;
	m_tInfoWeapon.matParent = m_tInfo.matWorld;
	m_tInfoHead.matParent = m_tInfo.matWorld;

// Balloon
	auto iterBalloon = m_listBalloon.begin();
	while (iterBalloon != m_listBalloon.end()) {
		if ((*iterBalloon)->GetDead()) {
			(*iterBalloon)->Release();
			delete (*iterBalloon);
			iterBalloon = m_listBalloon.erase(iterBalloon);
			continue;
		}
		(*iterBalloon)->LateUpdate();
		++iterBalloon;
	}
}

void CPlayer::Render()
{
	m_pImageBack->Render(&m_tInfoBack);
	m_pCurrentImage->Render(&m_tInfo);
	m_pImageWeapon->Render(&m_tInfoWeapon);
	m_pImageHead->Render(&m_tInfoHead);

	// Balloon
	auto iterBalloon = m_listBalloon.begin();
	while (iterBalloon != m_listBalloon.end()) {
		if ((*iterBalloon)->GetDead()) {
			++iterBalloon;
			continue;
		}
		(*iterBalloon)->Render();
		++iterBalloon;
	}
}

void CPlayer::Release()
{
}

void CPlayer::Attack()
{
	if (m_iBullet == 0) {
		TextBalloon_NoBullet();
		return;
	}

	float fCurrTime = TIMEMANAGER->GetWorldTime();
	if ((fCurrTime - m_fAttackLastTime) < m_fAttackPerSecond)
		return;

	m_fAttackLastTime = fCurrTime;

	// 플레이어 위치
	if (m_pd == PD_LEFT) {
		m_tInfo.vPos.x += 3.f;
	} else {
		m_tInfo.vPos.x -= 3.f;
	}
	
	// 총알
	float fAngle = m_pd == PD_LEFT ? 180.f : 0.f;
	fAngle += (rand() % 5) - 2.5f;

	_vec3 vPos = m_tInfo.vPos;
	vPos.y += 10.f;
	CBullet* pBullet = new CBullet(vPos, fAngle, 0);
	m_pBulletList->emplace_back(pBullet);

	--m_iBullet;

	SOUNDMANAGER->play("bullet");
}

void CPlayer::ChargeBullet()
{
	m_iBullet = m_iBulletMax;
}

void CPlayer::ActiveGravity()
{
	// 중력작용
	if (!m_bJump) {
		m_fGravity += GRAVITY_VALUE;
		m_tInfo.vPos.y += m_fGravity;
	}
	else {
		m_tInfo.vPos.y -= sinf(D3DXToRadian(m_fAngle)) * m_fForce;
		m_tInfo.vPos.y += m_fGravity;
		m_fGravity += GRAVITY_VALUE;
	}
}

void CPlayer::DeactivateGravity()
{
	if (m_bJump) {
		m_ps = PS_IDLE;
	}

	m_fGravity = 0.f;
	m_bJump = false;
	m_iJumpCount = 0;
}

void CPlayer::TextBalloon_NoBullet()
{
	CBalloon* pBalloon = new CBalloon;

	pBalloon->SetPlayer(this);

	if (m_pd == PD_RIGHT)
		pBalloon->SetFlip(true);

	m_listBalloon.emplace_back(pBalloon);
}

CBalloon::CBalloon()
{
	LJH::CImageManager* pImageManager = LJH::CImageManager::GetInstance();
	
	m_ImageBalloon = new LJH::CImage((*pImageManager->GetImage(L"bigsprites2", L"textballoon")));
	m_ImageBullet  = new LJH::CImage((*pImageManager->GetImage(L"weapons", L"bullet1")));
	m_ImageMark    = new LJH::CImage((*pImageManager->GetImage(L"sprites", L"questionmark")));

	m_fStartTime = TIMEMANAGER->GetWorldTime();
}

CBalloon::~CBalloon()
{
	Release();
}

void CBalloon::Ready()
{
}

void CBalloon::Update()
{
	float fCurrTime = TIMEMANAGER->GetWorldTime();
	if ((fCurrTime - m_fStartTime) > m_fShowTime) {
		SetDead(true);
	}

	m_ImageBalloon->Update();
	m_ImageBullet->Update();
	m_ImageMark->Update();
}

void CBalloon::LateUpdate()
{
}

void CBalloon::Render()
{
	INFO infoBalloon;
	INFO infoBullet;
	INFO infoMark;
	INFO infoPlayer = m_pPlayer->GetInfo();

	infoBalloon.vPos = infoPlayer.vPos;
	infoBullet.vPos = infoPlayer.vPos;
	infoMark.vPos = infoPlayer.vPos;

	if (m_bFlip) {
		infoBalloon.vPos.x += 70.f;
		infoBullet.vPos.x += 90.f;
		infoMark.vPos.x += 60.f;
		m_ImageBalloon->SetFlipY(true);
		
	}
	else {
		infoBalloon.vPos.x -= 70.f;
		infoBullet.vPos.x -= 90.f;
		infoMark.vPos.x -= 60.f;

	}
	infoBalloon.vPos.y -= 70.f;
	infoBalloon.color = D3DCOLOR_ARGB(255, 70, 135, 134);

	infoBullet.vPos.y -= 85.f;
	infoBullet.vScale = { 0.5f, 0.5f, 0.f };
	infoBullet.color = D3DCOLOR_ARGB(255, 205, 255, 255);

	infoMark.vPos.y -= 85.f;
	infoMark.vScale = { 0.5f, 0.5f, 0.f };
	infoMark.color = D3DCOLOR_ARGB(255, 205, 255, 255);

	m_ImageBalloon->Render(&infoBalloon);
	m_ImageBullet->Render(&infoBullet);
	m_ImageMark->Render(&infoMark);
}

void CBalloon::Release()
{
	SafeDelete(m_ImageBalloon);
	SafeDelete(m_ImageBullet);
	SafeDelete(m_ImageMark);
}
