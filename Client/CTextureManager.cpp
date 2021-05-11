#include "stdafx.h"
#include "CTexture.h"
#include "CTextureManager.h"

IMPLEMENT_SINGLETON(CTextureManager)
CTextureManager::CTextureManager()
{
}


CTextureManager::~CTextureManager()
{
	Release();
}

const TEXINFO * CTextureManager::GetTexInfo(const wstring & strObjectKey)
{
	auto& iter_find = m_mapTexture.find(strObjectKey);

	if (m_mapTexture.end() == iter_find)
		return nullptr;
	else
		return iter_find->second->GetTexInfo();
}

CTexture * CTextureManager::GetTexture(const wstring & strObjectKey)
{
	auto& iter_find = m_mapTexture.find(strObjectKey);

	if (m_mapTexture.end() == iter_find)
		return nullptr;
	else
		return iter_find->second;
}

HRESULT CTextureManager::InsertTexture(const wstring& strFilePath, const wstring & strObjectKey, const int& iIndexMaxX, const int& iIndexMaxY)
{
	map<wstring, CTexture*>::iterator iter_find = m_mapTexture.find(strObjectKey);

	if (iter_find != m_mapTexture.end()) return E_FAIL;
	
	CTexture* pTexture = new CTexture(iIndexMaxX, iIndexMaxY);
	if (FAILED(pTexture->InsertTexture(strFilePath)))
	{
		ERR_MSG(L"Texture Creating Failed");
		delete pTexture;
		return E_FAIL;
	}
	m_mapTexture.emplace(strObjectKey, pTexture);
	
	return S_OK;
}

void CTextureManager::Release()
{
	for (auto& rPair : m_mapTexture)
		SafeDelete(rPair.second);
	
	m_mapTexture.clear();
}
