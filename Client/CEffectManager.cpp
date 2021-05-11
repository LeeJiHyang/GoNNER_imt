#include "stdafx.h"
#include "CEffectManager.h"
#include "CImageManager.h"
#include "CEffect.h"

void CEffectManager::Init()
{
}

void CEffectManager::Release()
{
	list<CEffect*>::iterator iter = m_listEffect.begin();
	while (iter != m_listEffect.end()) {
		iter = m_listEffect.erase(iter);
	}
	m_listEffect.clear();
}

int CEffectManager::Update()
{
	list<CEffect*>::iterator iter = m_listEffect.begin();
	LJH::CImage* pImage = nullptr;
	while (iter != m_listEffect.end()) {
		pImage = (*iter)->m_Image;
		if (pImage->GetPS() != ANI_NOLOOP) { iter++; continue; }
		if (pImage->GetRepeatNum() >= 1) {
			iter = m_listEffect.erase(iter);
			continue;
		}
		iter++;
	}

	return 0;
}

void CEffectManager::LateUpdate()
{
}

void CEffectManager::Render()
{
	for (auto pEffect : m_listEffect) {
		pEffect->Render();
	}
}

void CEffectManager::AddEffect(const wstring& strTextureKey, const wstring& strImageKey, INFO info)
{
	LJH::CImageManager* pImageManager = LJH::CImageManager::GetInstance();

	CEffect* eff = new CEffect;

	LJH::CImage* pImage = new LJH::CImage(*pImageManager->GetImage(strTextureKey, strImageKey));
	pImage->SetCurrentFrame(0);
	pImage->SetRepeatNum(0);

	eff->m_Image = pImage;
	eff->SetInfo(info);

	m_listEffect.emplace_back(eff);
}

vector<RECT> CEffectManager::GetVecRectCol()
{
	vector<RECT> vecCol;
	list<CEffect*>::iterator iter = m_listEffect.begin();

	while (iter != m_listEffect.end()) {
		RECT rc = (*iter)->GetCol();
		vecCol.emplace_back(rc);
		iter++;
	}

	return vecCol;
}

list<RECT*> CEffectManager::GetListRectCol()
{
	list<RECT*> listCol;
	list<CEffect*>::iterator iter = m_listEffect.begin();

	while (iter != m_listEffect.end()) {
		RECT rc = (*iter)->GetCol();
		listCol.emplace_back(&rc);
		iter++;
	}

	return listCol;
}
