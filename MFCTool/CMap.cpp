#include "stdafx.h"
#include "CTextureManager.h"
#include "CImageManager.h"
#include "MFCToolView.h"
#include "CImage.h"
#include "CMap.h"

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

	m_ImagePortalIn = pImgMgr->GetImage(L"bigsprites2", L"portalin_idle");
	m_ImagePortalOut = pImgMgr->GetImage(L"bigsprites2", L"portalout_idle");
	m_ImageMonster[0] = pImgMgr->GetImage(L"smallenemies", L"enemy00_idle");
	m_ImageMonster[1] = pImgMgr->GetImage(L"smallenemies", L"enemy01_idle");

	CMapData mapdata;
	m_vecMapData.emplace_back(mapdata);

	// Load Map Data
	//LoadMapData();
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
			ReadFile(hHandle, &line, sizeof(LINE), &dwByte, nullptr);
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

HRESULT LJH::CMap::Ready()
{
	return S_OK;
}

void LJH::CMap::Render()
{
	CGraphicDevice* pGraphicDevice = CGraphicDevice::GetInstance();

	// Line
	{
		vector<LINE>::iterator iter = m_vecMapData[m_iMapSel].m_vecLine.begin();
		while (iter != m_vecMapData[m_iMapSel].m_vecLine.end())
		{
			INFO info;
			info.vPos.x = iter->vPos.x - m_pView->GetScrollPos(0);
			info.vPos.y = iter->vPos.y - m_pView->GetScrollPos(1);
			info.vRot.z = iter->dir == 0 ? 0.f :
				iter->dir == 1 ? 90.f :
				iter->dir == 2 ? 180.f :
				270.f;
			m_ImageLine[m_iMapSel][0]->Render(&info);
			iter++;
		}
	}

	// Portal - in
	{
		INFO info;
		info.vPos.x = (float)m_vecMapData[m_iMapSel].m_ptPortalIn.x - m_pView->GetScrollPos(0);
		info.vPos.y = (float)m_vecMapData[m_iMapSel].m_ptPortalIn.y - m_pView->GetScrollPos(1);
		info.vRot.z = 180.f;
		m_ImagePortalIn->Render(&info);
	}

	// Portal - out
	{
		INFO info;
		info.vPos.x = (float)m_vecMapData[m_iMapSel].m_ptPortalOut.x - m_pView->GetScrollPos(0);
		info.vPos.y = (float)m_vecMapData[m_iMapSel].m_ptPortalOut.y - m_pView->GetScrollPos(1);
		m_ImagePortalOut->Render(&info);
	}

	// Monster
	{
		vector<CMobData>::iterator iter = m_vecMapData[m_iMapSel].m_vecMob.begin();
		while (iter != m_vecMapData[m_iMapSel].m_vecMob.end())
		{
			INFO info;
			info.vPos.x = iter->m_vPos.x - m_pView->GetScrollPos(0);
			info.vPos.y = iter->m_vPos.y - m_pView->GetScrollPos(1);
			m_ImageMonster[iter->m_iMobID]->Render(&info);
			iter++;
		}
	}

}

void LJH::CMap::Release()
{

}
