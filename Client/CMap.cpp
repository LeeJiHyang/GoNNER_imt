#include "stdafx.h"
#include "CTextureManager.h"
#include "CImageManager.h"
#include "CCameraManager.h"
#include "CImage.h"
#include "CMap.h"
#include "CMonster.h"

const int iPortalBodyNum = 20;

LJH::CMap::CMap()
{
	CImageManager* pImgMgr = CImageManager::GetInstance();

	m_ImageLine[0][0] = pImgMgr->GetImage(L"line", L"line00_0");
	m_ImageLine[0][1] = pImgMgr->GetImage(L"line", L"line00_1");
	m_ImageLine[0][2] = pImgMgr->GetImage(L"line", L"line00_2");
	m_ImageLine[0][3] = pImgMgr->GetImage(L"line", L"line00_3");
	m_ImageLine[0][4] = pImgMgr->GetImage(L"line", L"line00_4");
	m_ImageLine[0][5] = pImgMgr->GetImage(L"line", L"line00_5");
	m_ImageLine[0][6] = pImgMgr->GetImage(L"line", L"line00_6");
	m_ImageLine[0][7] = pImgMgr->GetImage(L"line", L"line00_7");
	m_ImageLine[0][8] = pImgMgr->GetImage(L"line", L"line00_8");

	CMapData mapdata;
	m_vecMapData.emplace_back(mapdata);

	m_objID = OBJ::OBJ_MAP;

	// Portal
	m_pPortalIn  = new CPortal;
	m_pPortalIn->SetType(PORTAL_IN);
	m_pPortalIn->SetRatio(0.7f);

	m_pPortalOut = new CPortal;
	m_pPortalOut->SetType(PORTAL_OUT);
	m_pPortalOut->SetRatio(0.7f);

	// Load Map Data
	LoadMapData();
}


LJH::CMap::~CMap()
{
	Release(); 
}

void LJH::CMap::LoadMapData()
{
	HANDLE hHandle = CreateFile(L"C:\\Users\\LJH\\Documents\\Dev\\GoNNER_dx9\\resource\\map\\mapdata0.dat", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	DWORD dwByte = 0;
	int iSize = 0;

	for (int i = 0; i < 1; i++) {
		CMapData* pMapData = &m_vecMapData[i];
		// Type
		ReadFile(hHandle, &pMapData->m_iType, sizeof(int), &dwByte, nullptr);
		// Line 개수
		ReadFile(hHandle, &iSize, sizeof(int), &dwByte, nullptr);
		pMapData->m_vecLine.reserve(iSize);
		// Line 위치(LINE)
		LINE line;
		for (int i = 0; i < iSize; i++)
		{
			ReadFile(hHandle, &line, sizeof(_vec3) + sizeof(LINE_DIR), &dwByte, nullptr);
			pMapData->m_vecLine.emplace_back(line);
		}
		// Collision 개수
		ReadFile(hHandle, &iSize, sizeof(int), &dwByte, nullptr);
		pMapData->m_vecColl.reserve(iSize);
		// Collision 위치(RECT)
		RECT rc;
		for (int i = 0; i < iSize; i++)
		{
			ReadFile(hHandle, &rc, sizeof(RECT), &dwByte, nullptr);
			pMapData->m_vecColl.emplace_back(rc);
		}
		// Mob 개수
		ReadFile(hHandle, &iSize, sizeof(int), &dwByte, nullptr);
		pMapData->m_vecMob.reserve(iSize);
		// Mob 위치(CMobData)
		CMobData mobdata;
		for (int i = 0; i < iSize; i++)
		{
			ReadFile(hHandle, &mobdata, sizeof(CMobData), &dwByte, nullptr);
			pMapData->m_vecMob.emplace_back(mobdata);
		}
		// Portal In 위치(POINT)
		ReadFile(hHandle, &pMapData->m_ptPortalIn, sizeof(POINT), &dwByte, nullptr); // 글자 개수	
		// Portal Out 위치(POINT)
		ReadFile(hHandle, &pMapData->m_ptPortalOut, sizeof(POINT), &dwByte, nullptr); // 글자 개수
	}

	CloseHandle(hHandle);
}

void LJH::CMap::SaveMapData(HANDLE hHandle, int iMapSel)
{
	DWORD dwByte = 0;
	int iSize = 0;
	CMapData* pMapData = &m_vecMapData[iMapSel];

	// Type
	WriteFile(hHandle, &pMapData->m_iType, sizeof(int), &dwByte, nullptr); // 글자 개수

	// Line 개수
	iSize = pMapData->m_vecLine.size();
	WriteFile(hHandle, &iSize, sizeof(int), &dwByte, nullptr); // 글자 개수

	// Line 위치(LINE)
	for (int i = 0; i < iSize; i++) {
		LINE line = pMapData->m_vecLine[i];
		WriteFile(hHandle, &line, sizeof(LINE), &dwByte, nullptr);
	}

	// Collision 개수
	iSize = pMapData->m_vecColl.size();
	WriteFile(hHandle, &iSize, sizeof(int), &dwByte, nullptr); // 글자 개수

	// Collision 위치(RECT)
	for (int i = 0; i < iSize; i++) {
		RECT rc = pMapData->m_vecColl[i];
		WriteFile(hHandle, &rc, sizeof(RECT), &dwByte, nullptr);
	}

	// Mob 개수
	iSize = pMapData->m_vecMob.size();
	WriteFile(hHandle, &iSize, sizeof(int), &dwByte, nullptr); // 글자 개수

	// Mob 위치(POINT)
	for (int i = 0; i < iSize; i++) {
		CMobData mobdata = pMapData->m_vecMob[i];
		WriteFile(hHandle, &mobdata, sizeof(CMobData), &dwByte, nullptr);
	}

	// Portal In 위치(POINT)
	WriteFile(hHandle, &pMapData->m_ptPortalIn, sizeof(POINT), &dwByte, nullptr); // 글자 개수	

	// Portal Out 위치(POINT)
	WriteFile(hHandle, &pMapData->m_ptPortalOut, sizeof(POINT), &dwByte, nullptr); // 글자 개수
}

void LJH::CMap::ChangeMapData(int iSel)
{
	SafeDelete(m_pCurrMapData);
	m_pCurrMapData = new CMapData;

	// Type
	m_pCurrMapData->m_iType = m_vecMapData[iSel].m_iType;
	
	// Line
	m_pCurrMapData->m_vecLine.reserve(m_vecMapData[iSel].m_vecLine.size());
	for (size_t i = 0; i < m_vecMapData[iSel].m_vecLine.size(); ++i) {
		LINE line = m_vecMapData[iSel].m_vecLine[i];
		line.vPos.x *= m_fRatio;
		line.vPos.y *= m_fRatio;
		for (int i = 0; i < 3; i++) {
			line.iImageNum[i] = rand() % 6;
			// Angle
			line.vAngle[i].z = line.dir == 0 ?   0.f :
				               line.dir == 1 ?  90.f :
				               line.dir == 2 ? 180.f :
				                               270.f;
			line.vAngle[i].z += ((rand() % 20) - 10);
			line.vStartAngle[i].z = line.vAngle[i].z + (float)(rand() % 180 - 90);
			// Scale
			line.vScale[i] = { m_fRatio, m_fRatio, 0.f };
			line.vStartScale[i] = { 0.3f, 0.1f, 0.f };
		}
		m_pCurrMapData->m_vecLine.emplace_back(line);
	}
	
	// Monster
	// Clear
	auto iterMonster = m_pMonsterList->begin();
	while (iterMonster != m_pMonsterList->end()) {
		SafeDelete(*iterMonster);
		iterMonster++;
	}
	m_pMonsterList->clear();
	// Add Monster
	m_pCurrMapData->m_vecMob.reserve(m_vecMapData[iSel].m_vecMob.size());
	for (size_t i = 0; i < m_vecMapData[iSel].m_vecMob.size(); ++i) {
		CMobData mobdata = m_vecMapData[iSel].m_vecMob[i];
		mobdata.m_vPos.x *= m_fRatio;
		mobdata.m_vPos.y *= m_fRatio;
		m_pCurrMapData->m_vecMob.emplace_back(mobdata);
		
		CObj* pMonster = new CMonster(mobdata.m_vPos, mobdata.m_iMobID);
		m_pMonsterList->emplace_back(pMonster);
	}
	
	// Coll
	m_pCurrMapData->m_vecColl.reserve(m_vecMapData[iSel].m_vecColl.size());
	for (size_t i = 0; i < m_vecMapData[iSel].m_vecColl.size(); ++i) {
		RECT rc = m_vecMapData[iSel].m_vecColl[i];
		rc.left   = (LONG)(rc.left   * m_fRatio);
		rc.top    = (LONG)(rc.top    * m_fRatio);
		rc.right  = (LONG)(rc.right  * m_fRatio);
		rc.bottom = (LONG)(rc.bottom * m_fRatio);
		m_pCurrMapData->m_vecColl.emplace_back(rc);
		// Coll Manager 추가 필요
	}

	// Portal In
	m_pCurrMapData->m_ptPortalIn = m_vecMapData[iSel].m_ptPortalIn;
	m_pCurrMapData->m_ptPortalIn.x = (LONG)(m_pCurrMapData->m_ptPortalIn.x * m_fRatio);
	m_pCurrMapData->m_ptPortalIn.y = (LONG)(m_pCurrMapData->m_ptPortalIn.y * m_fRatio);
	m_pPortalIn->SetPos((float)m_pCurrMapData->m_ptPortalIn.x, (float)m_pCurrMapData->m_ptPortalIn.y);

	// Portal Out
	m_pCurrMapData->m_ptPortalOut = m_vecMapData[iSel].m_ptPortalOut;
	m_pCurrMapData->m_ptPortalOut.x = (LONG)(m_pCurrMapData->m_ptPortalOut.x * m_fRatio);
	m_pCurrMapData->m_ptPortalOut.y = (LONG)(m_pCurrMapData->m_ptPortalOut.y * m_fRatio);
	m_pPortalOut->SetPos((float)m_pCurrMapData->m_ptPortalOut.x, (float)m_pCurrMapData->m_ptPortalOut.y);

	// Map Size
	float fXMax = 0.f;
	float fYMax = 0.f;
	float fCX = TILEGAP * m_fRatio;
	float fCY = TILEGAP * m_fRatio * 0.1f;
	for (size_t i = 0; i < m_pCurrMapData->m_vecLine.size(); ++i) {
		if (fXMax < (m_pCurrMapData->m_vecLine[i].vPos.x + (fCX * 0.5f))) {
			fXMax = (m_pCurrMapData->m_vecLine[i].vPos.x + (fCX * 0.5f));
		}
		if (fYMax < (m_pCurrMapData->m_vecLine[i].vPos.y + (fCY * 0.5f))) {
			fYMax = (m_pCurrMapData->m_vecLine[i].vPos.y + (fCY * 0.5f));
		}
	}
	m_tInfo.vSize.x = fXMax;
	m_tInfo.vSize.y = fYMax;
}

void LJH::CMap::Ready()
{
}

void LJH::CMap::Render()
{
	CGraphicDevice* pGraphicDevice = CGraphicDevice::GetInstance();
	CCameraManager* pCamMgr = CCameraManager::GetInstance();

	// Line
	{
		vector<LINE>::iterator iter = m_pCurrMapData[m_iMapSel].m_vecLine.begin();
		while (iter != m_pCurrMapData[m_iMapSel].m_vecLine.end())
		{
			INFO info;
			info.vPos.x = iter->vPos.x;
			info.vPos.y = iter->vPos.y;

			info.color = iter->color;

			for (int i = 2; i >= 0; --i) {
				if (!iter->bDraw[i]) continue;

				INFO infoAfter = info;

				switch (iter->dir) {
				case LINE_DIR_U:
					infoAfter.vPos.y += 15 * i;
					break;
				case LINE_DIR_D:
					infoAfter.vPos.y -= 15 * i;
					break;
				case LINE_DIR_L:
					infoAfter.vPos.x += 15 * i;
					break;
				case LINE_DIR_R:
					infoAfter.vPos.x -= 15 * i;
					break;
				}
				// Scale
				D3DXVec3Lerp(&iter->vShowScale[i], &iter->vShowScale[i], &iter->vScale[i], 0.1f);
				infoAfter.vScale = iter->vShowScale[i];
				// Angle
				D3DXVec3Lerp(&iter->vShowAngle[i], &iter->vShowAngle[i], &iter->vAngle[i], 0.1f);
				infoAfter.vRot = iter->vShowAngle[i];
				
				int r, g, b;
				r = ((info.color >> 16) & 0xff) - (30 * i);
				g = ((info.color >>  8) & 0xff) - (30 * i);
				b = ((info.color >>  0) & 0xff) - (30 * i);
				if (r < 0) r = 0;
				if (g < 0) g = 0;
				if (b < 0) b = 0;
				
				infoAfter.color = D3DCOLOR_ARGB(255, r, g, b);

				m_ImageLine[m_iMapSel][iter->iImageNum[i]]->Render(&infoAfter);
			}

			iter++;
		}
	}

	m_pPortalIn->Render();
	m_pPortalOut->Render();
}

void LJH::CMap::Update()
{
	m_pPortalIn->Update();
	m_pPortalOut->Update();
}

void LJH::CMap::LateUpdate()
{
	m_pPortalIn->LateUpdate();
	m_pPortalOut->LateUpdate();
}

void LJH::CMap::Release()
{
	SafeReleaseDelete(m_pPortalIn);
	SafeReleaseDelete(m_pPortalOut);
}

void LJH::CMap::SetMonsterList(list<CObj*>* pMonsterList)
{
	m_pMonsterList = pMonsterList;
}

CPortal::CPortal()
{
// Info
	// Color
	m_tInfo.color = D3DCOLOR_ARGB(255, 83, 180, 184);
	m_infoDraw.color = m_tInfo.color;
	m_infoGoal.color = m_tInfo.color;

// Create Body
	for (int i = 0; i < iPortalBodyNum; i++) {
		m_listBody.emplace_back(CPortalBody());
		m_listAngleBuff.emplace_back(0.f);
	}
}

CPortal::~CPortal()
{
}

void CPortal::Ready()
{
}

void CPortal::Update()
{
	// Angle
	float fCurrTime = TIMEMANAGER->GetWorldTime();
	if ((fCurrTime - m_fLastAngleChange) > m_fTimeAngleChange) {
		m_fLastAngleChange = fCurrTime;
		m_fChangeAngle *= -1;
		m_infoGoal.vRot.z = m_tInfo.vRot.z + m_fChangeAngle;
	}

	D3DXVec3Lerp(&m_infoDraw.vRot, &m_infoDraw.vRot, &m_infoGoal.vRot, 0.01f);

	m_listAngleBuff.pop_back();
	m_listAngleBuff.emplace_front(m_infoDraw.vRot.z);

	// Body
	list<CPortalBody>::iterator iterBody = m_listBody.begin();
	list<float>::iterator iterAngleBuff = m_listAngleBuff.begin();
	INFO info = m_tInfo;

	// 위치
	float fAngle = m_infoDraw.vRot.z + 90.f; // 바디에 붙는 위치를 구하기 위한 각도
	float fBodyPos;
	if (m_Type == PORTAL_IN) { fBodyPos = 15.f; }
	else { fBodyPos = 20.f; }
	info.vPos.x += cosf(D3DXToRadian(fAngle)) * fBodyPos; // 기차놀이 시작위치
	info.vPos.y += sinf(D3DXToRadian(fAngle)) * fBodyPos; // 기차놀이 시작위치
	// 크기
	info.vScale = { 0.7f, 0.7f, 0.f };
	// 자전
	while (iterBody != m_listBody.end()) {
		fAngle = (*iterAngleBuff) + 90.f;
		info.vPos.x += cosf(D3DXToRadian(fAngle)) * 50.f;
		info.vPos.y += sinf(D3DXToRadian(fAngle)) * 50.f;
		info.vRot.z = (*iterAngleBuff);
		iterBody->SetInfo(info);
		iterBody->Update();

		iterBody++;
		iterAngleBuff++;
	}

	m_ImageHead->Update();
}

void CPortal::LateUpdate()
{
}

void CPortal::Render()
{
	list<CPortalBody>::iterator iterBody = m_listBody.begin();
	while (iterBody != m_listBody.end()) {
		iterBody->Render();
		iterBody++;
	}
	m_ImageHead->Render(&m_infoDraw);
}

void CPortal::Release()
{
	list<CPortalBody>::iterator iterBody = m_listBody.begin();
	while (iterBody != m_listBody.end()) {
		iterBody->Release();
		iterBody = m_listBody.erase(iterBody);
	}
	m_listBody.clear();
}

void CPortal::SetType(PORTAL_TYPE type)
{
	LJH::CImageManager* pImgMgr = LJH::CImageManager::GetInstance();
	m_Type = type;

	switch (type)
	{
	case PORTAL_IN:
		m_ImageHead = pImgMgr->GetImage(L"bigsprites2", L"portalin_idle");
		m_tInfo.vRot.z = 180.f;
		break;

	case PORTAL_OUT:
		m_ImageHead = pImgMgr->GetImage(L"bigsprites2", L"portalout_idle");
		break;
	}

	m_infoDraw.vRot = m_tInfo.vRot;
	m_infoGoal.vRot = m_tInfo.vRot;
}

void CPortal::SetRatio(float fRatio)
{
	m_fRatio = fRatio;

	m_tInfo.vScale.x = m_fRatio;
	if (m_Type == PORTAL_IN) m_tInfo.vScale.x *= 0.7f;
	m_tInfo.vScale.y = m_fRatio;

	m_infoDraw.vScale = m_tInfo.vScale;
	m_infoGoal.vScale = m_tInfo.vScale;
}

void CPortal::SetPos(float fX, float fY)
{
	m_tInfo.vPos.x = fX;
	m_tInfo.vPos.y = fY;

	m_infoDraw.vPos = m_tInfo.vPos;
	m_infoGoal.vPos = m_tInfo.vPos;
}

CPortalBody::CPortalBody()
{
	LJH::CImageManager* pImageManager = LJH::CImageManager::GetInstance();

	m_Image = new LJH::CImage(*pImageManager->GetImage(L"bigsprites2", L"portalbody_3"));
}

CPortalBody::~CPortalBody()
{
}

void CPortalBody::Ready()
{
}

void CPortalBody::Update()
{
	m_Image->Update();
}

void CPortalBody::LateUpdate()
{
}

void CPortalBody::Render()
{
	m_Image->Render(&m_tInfo);
}

void CPortalBody::Release()
{
	SafeDelete(m_Image);
}
