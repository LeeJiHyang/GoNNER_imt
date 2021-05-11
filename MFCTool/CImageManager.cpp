#include "stdafx.h"
#include "CTextureManager.h"
#include "CTexture.h"
#include "CImage.h"
#include "CImageManager.h"

IMPLEMENT_SINGLETON(CImageManager)

CImageManager::CImageManager()
{
}

CImageManager::~CImageManager()
{
	Release();
}

void CImageManager::Release()
{
	map<wstring, CTextureKey>::iterator iter = m_mapTextureKey.begin();

	while (iter != m_mapTextureKey.end())
	{
		iter->second.Release();
		++iter;
	}
	m_mapTextureKey.clear();
}

HRESULT CImageManager::InsertImage(const wstring& strTextureKey, const wstring& strImageKey, const int& iStartIndexX, const int& iStartIndexY, const int& iMaxFrame)
{
	CTextureManager* pTextureManager = CTextureManager::GetInstance();
	CTexture* pTexture = pTextureManager->GetTexture(strTextureKey);
	if (pTexture == nullptr) return E_FAIL;

	LJH::CImage* pImage = new LJH::CImage(pTexture, strImageKey, iStartIndexX, iStartIndexY, iMaxFrame);

	map<wstring, CTextureKey>::iterator iter_find = m_mapTextureKey.find(strTextureKey);
	if (iter_find == m_mapTextureKey.end()) {
		m_mapTextureKey.emplace(strTextureKey, CTextureKey());
		iter_find = m_mapTextureKey.find(strTextureKey);
	}

	iter_find->second.InsertImage(strImageKey, pImage);

	return S_OK;
}

LJH::CImage * CImageManager::GetImage(const wstring & strTextureKey, const wstring & strImageKey)
{
	CTextureManager* pTextureManager = CTextureManager::GetInstance();
	CTexture* pTexture = pTextureManager->GetTexture(strTextureKey);
	if (pTexture == nullptr) return nullptr;

	auto iter = m_mapTextureKey.find(strTextureKey);
	if (iter == m_mapTextureKey.end()) return nullptr;

	LJH::CImage* pImage = iter->second.GetImage(strImageKey);
	if (pImage == nullptr) return nullptr;
	
	return pImage;
}

CTextureKey::CTextureKey()
{
}

CTextureKey::~CTextureKey()
{
}

void CTextureKey::Release()
{
	auto iter = m_mapImage.begin();
	while (iter != m_mapImage.end())
	{
		delete iter->second;
		iter++;
	}
	m_mapImage.clear();
}

HRESULT CTextureKey::InsertImage(const wstring & strImageKey, LJH::CImage* pImage)
{
	m_mapImage.emplace(strImageKey, pImage);

	return S_OK;
}

LJH::CImage * CTextureKey::GetImage(const wstring & strImageKey)
{
	auto iter = m_mapImage.find(strImageKey);
	
	if (iter == m_mapImage.end()) return nullptr;
	return iter->second;
}
