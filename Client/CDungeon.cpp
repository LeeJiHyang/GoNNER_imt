#include "stdafx.h"
#include "CDungeon.h"
#include "CObj.h"
#include "CPlayer.h"
#include "CMonster.h"
#include "CMap.h"
#include "CEffect.h"
#include "CBubble.h"
#include "CCameraManager.h"
#include "CCollisionManager.h"
#include "CImageManager.h"

bool CDungeon::Ready()
{
	SOUNDMANAGER->play("bgm");

// Set Object
	m_pPlayer = dynamic_cast<CPlayer*>(*m_listObject[OBJ::OBJ_PLAYER].begin());
	m_pPlayer->SetBulletList(&m_listObject[OBJ::OBJ_BULLET]);
	m_pPlayer->SetEffectList(&m_listObject[OBJ::OBJ_EFFECT]);
	m_pMap = dynamic_cast<LJH::CMap*>(*m_listObject[OBJ::OBJ_MAP].begin());
	m_pMap->SetMonsterList(&m_listObject[OBJ::OBJ_MONSTER]);
	ChangeMap(0);
	auto iterMonster = m_listObject[OBJ::OBJ_MONSTER].begin();
	while (iterMonster != m_listObject[OBJ::OBJ_MONSTER].end()) {
		dynamic_cast<CMonster*>(*iterMonster)->SetEffectList(&m_listObject[OBJ::OBJ_EFFECT]);
		++iterMonster;
	}

	BUBBLEMANAGER->SetBubbleList(&m_listObject[OBJ::OBJ_BUBBLE]);

	return true;
}

void CDungeon::Release()
{
}

void CDungeon::Update()
{
	for (int i = 0; i < OBJ::OBJ_END; i++) {
		for (list<CObj*>::iterator iter = m_listObject[i].begin(); iter != m_listObject[i].end(); iter++) {
			(*iter)->Update();
		}
	}
}

void CDungeon::LateUpdate()
{
	LJH::CImageManager* pImageManager = LJH::CImageManager::GetInstance();

	// Object Late Update
	for (int i = 0; i < OBJ::OBJ_END; i++) {
		for (list<CObj*>::iterator iter = m_listObject[i].begin(); iter != m_listObject[i].end(); ++iter) {
			(*iter)->LateUpdate();
		}
	}

// 충돌
	// 맵 / 플레이어 충돌
	vector<RECT> vecRectCol = m_pMap->GetVecCol();
	if (CCollisionManager::CollisionRectEX(vecRectCol, m_listObject[OBJ::OBJ_PLAYER])) {
		m_pPlayer->DeactivateGravity();
	}

	// 맵 / 몬스터 충돌
	if (CCollisionManager::CollisionRectEX(vecRectCol, m_listObject[OBJ::OBJ_MONSTER])) {
		// CollisionRectEX 함수 내 처리
	}

	// 몬스터 / 총알 충돌
	{
		CCollisionManager::CollisionRect(m_listObject[OBJ::OBJ_MONSTER], m_listObject[OBJ::OBJ_BULLET]);
		
	}

	// 총알 / 맵 충돌
	{
		RECT rc = {};
		vector<RECT> vecLineCol = m_pMap->GetVecCol();
		vector<RECT>::iterator iterLine = vecLineCol.begin();
		while (iterLine != vecLineCol.end())
		{
			list<CObj*>::iterator iterBullet = m_listObject[OBJ::OBJ_BULLET].begin();
			while (iterBullet != m_listObject[OBJ::OBJ_BULLET].end())
			{
				if ((*iterBullet)->GetDeadStart()) {
					++iterBullet;
					continue;
				}
				if (IntersectRect(&rc, &(*iterLine), &(*iterBullet)->GetCol()))
				{
					AddEffect_BulletHitLine(rc, (*iterBullet));
					// 총알 삭제
					SafeDelete((*iterBullet));
					iterBullet = m_listObject[OBJ::OBJ_BULLET].erase(iterBullet);
					SOUNDMANAGER->play("wallhit");
					continue;
				}
				++iterBullet;
			}
			++iterLine;
		}
	}


// 삭제
	// Dead가 True인 Object 삭제
	{
		for (int i = 0; i < OBJ::OBJ_END; ++i) {
			list<CObj*>::iterator iter = m_listObject[i].begin();
			while (iter != m_listObject[i].end())
			{
				if ((*iter)->GetDead()) {
					(*iter)->Release();
					delete (*iter);
					iter = m_listObject[i].erase(iter);
					continue;
				}
				iter++;
			}
		}
	}

	// 맵 범위 밖 총알 삭제
	// 루프가 끝난 NOLOOP 이펙트 삭제
	{
		list<CObj*>::iterator iterEffect = m_listObject[OBJ::OBJ_EFFECT].begin();
		while (iterEffect != m_listObject[OBJ::OBJ_EFFECT].end()) {
			if (dynamic_cast<CEffect*>(*iterEffect)->m_Image->GetPS() == ANI_NOLOOP) {
				if (dynamic_cast<CEffect*>(*iterEffect)->m_Image->GetRepeatNum() >= 1) {
					(*iterEffect)->Release();
					delete (*iterEffect);
					iterEffect = m_listObject[OBJ::OBJ_EFFECT].erase(iterEffect);
					continue;
				}
			}
			++iterEffect;
		}

		list<CObj*>::iterator iterBubble = m_listObject[OBJ::OBJ_BUBBLE].begin();
		while (iterBubble != m_listObject[OBJ::OBJ_BUBBLE].end()) {
			if (dynamic_cast<CBubble*>(*iterBubble)->m_Image->GetPS() == ANI_NOLOOP) {
				if (dynamic_cast<CBubble*>(*iterBubble)->m_Image->GetRepeatNum() >= 1) {
					(*iterBubble)->Release();
					delete (*iterBubble);
					iterBubble = m_listObject[OBJ::OBJ_BUBBLE].erase(iterBubble);
					continue;
				}
			}
			++iterBubble;
		}
	}

// 맵 Draw
	// 표현할 색상
	OBJ::ID objForMap[] = {
		OBJ::OBJ_PLAYER,
		OBJ::OBJ_MONSTER,
		OBJ::OBJ_EFFECT
	};
	int iSizeObjForMap = sizeof(objForMap) / sizeof(OBJ::ID);
	_vec3 vObj  = { 0.f, 0.f, 0.f };
	_vec3 vLine = { 0.f, 0.f, 0.f };
	float fDist = 0.f;
	bool bDraw[3/*Object*/][3/*Line*/] = {
	{ false, false, false },
	{ false, false, false },
	{ false, false, false } };
	BYTE byOnObj[3] = { 0, 0, 0 }; // 색상 섞을 때 가중치
	int iLineColor[3][4] = { // 선 색상
		{ 255, 50, 96, 78 },
		{ 255, 131, 38, 54 },
		{ 255, 140, 116, 54 }
	};

	for (vector<LINE>::iterator iterLine = m_pMap->m_pCurrMapData->m_vecLine.begin(); iterLine != m_pMap->m_pCurrMapData->m_vecLine.end(); ++iterLine) {
		vLine = (*iterLine).vPos;
		// false에서 처음 true가 됐을 때 라인 색상은 오브젝트 색상이 되어야 함
		// 계속 true인 상태이면 컬러가 변경되지 않음
		for (int i = 0; i < iSizeObjForMap; ++i) {
			bDraw[i][0] = false;
			bDraw[i][1] = false;
			bDraw[i][2] = false;
			byOnObj[i] = 0;
			for (list<CObj*>::iterator iterObj = m_listObject[objForMap[i]].begin(); iterObj != m_listObject[objForMap[i]].end(); ++iterObj) {
				vObj.x = (*iterObj)->GetX();
				vObj.y = (*iterObj)->GetY();
				fDist = GetDistance(vLine, vObj);

				bDraw[i][0] |= (fDist < 260 ? true : false);
				bDraw[i][1] |= (fDist < 180 ? true : false);
				bDraw[i][2] |= (fDist < 100 ? true : false);
			}
			byOnObj[i] = bDraw[i][2] ? 3 :
				         bDraw[i][1] ? 2 :
				         bDraw[i][0] ? 1 : 0;
		}
		for (int i = 0; i < 3; ++i) {
			if (!iterLine->bDraw[i] && bDraw[i]) {
				float fPoint = 0.f;
				int iR = 0;
				int iG = 0;
				int iB = 0;
				for (int j = 0; j < iSizeObjForMap; ++j) {
					fPoint += byOnObj[j];
					iR += (iLineColor[j][1] * byOnObj[j]);
					iG += (iLineColor[j][2] * byOnObj[j]);
					iB += (iLineColor[j][3] * byOnObj[j]);
				}
				iR = (int)(iR / fPoint);
				iG = (int)(iG / fPoint);
				iB = (int)(iB / fPoint);
				
				iterLine->color = D3DCOLOR_ARGB(255, iR, iG, iB);
				iterLine->vShowScale[i] = iterLine->vStartScale[i];
				iterLine->vShowAngle[i] = iterLine->vStartAngle[i];
			}

			bool bDrawAcc = false;
			for (int j = 0; j < iSizeObjForMap; ++j) {
				bDrawAcc |= bDraw[j][i];
			}
			iterLine->bDraw[i] = bDrawAcc;
		} 
	}
}

void CDungeon::Render()
{
	for (int i = 0; i < OBJ::OBJ_END; i++) {
		for (list<CObj*>::iterator iter = m_listObject[i].begin(); iter != m_listObject[i].end(); iter++) {
			(*iter)->Render();
		}
	}
}

void CDungeon::ChangeMap(int i)
{
	LJH::CMap* pMap = dynamic_cast<LJH::CMap*>(*m_listObject[OBJ::OBJ_MAP].begin());
	pMap->ChangeMapData(0);
	_vec3 vStartPos = m_pMap->GetStartPos();
	m_pPlayer->SetPos(vStartPos.x, vStartPos.y);
}

void CDungeon::AddEffect_BulletHitLine(RECT& rc, CObj* pBullet)
{
	LJH::CImageManager* pImageManager = LJH::CImageManager::GetInstance();

	float fAngle = pBullet->GetAngle() + 180.f;
// 1
	{
		// Info
		INFO info;
		info.vPos.x = (float)(rc.left + ((rc.right - rc.left) >> 1));
		info.vPos.y = (float)(rc.top + ((rc.bottom - rc.top) >> 1));
		info.vDir.x = +(cosf(D3DXToRadian(fAngle)) * 20.f);
		info.vDir.y = -(sinf(D3DXToRadian(fAngle)) * 20.f);
		info.vRot.z = fAngle + 90.f + ((rand() % 10) - 5);
		info.color = D3DCOLOR_ARGB(255, 255, 252, 80);

		// Image
		LJH::CImage* pImage = new LJH::CImage(*pImageManager->GetImage(L"bloodParticles", L"bullethitline1"));
		pImage->SetPS(ANI_NOLOOP);

		// Effect
		CEffect* pEffect = new CEffect();
		pEffect->m_Image = pImage;
		pEffect->SetInfo(info);

		m_listObject[OBJ::OBJ_EFFECT].emplace_back(pEffect);
	}

// 2
	{
		// Info
		INFO info;
		info.vPos.x = float(rc.left + ((rc.right - rc.left) >> 1));
		info.vPos.y = float(rc.top + ((rc.bottom - rc.top) >> 1));
		info.vDir.x = +(cosf(D3DXToRadian(fAngle)) * 200.f);
		info.vDir.y = -(sinf(D3DXToRadian(fAngle)) * 200.f);
		info.vRot.z = fAngle + 90.f + ((rand() % 10) - 5);
		info.color = D3DCOLOR_ARGB(255, 179, 141, 59);

		// Image
		LJH::CImage* pImage = new LJH::CImage(*pImageManager->GetImage(L"bloodParticles", L"bullethitline2"));
		pImage->SetPS(ANI_NOLOOP);

		// Effect
		CEffect* pEffect = new CEffect();
		pEffect->m_Image = pImage;
		pEffect->SetInfo(info);

		m_listObject[OBJ::OBJ_EFFECT].emplace_back(pEffect);
	}

	INFO info = pBullet->GetInfo();
	BUBBLEMANAGER->AddBubble((rand() % 3) + 1, info, pBullet->GetAngle(), D3DCOLOR_ARGB(255, 82, 53, 41));
	CCameraManager::GetInstance()->Shake(300);
}

void CDungeon::AddEffect_BulletHitMonster(CObj * pMonster)
{
}
