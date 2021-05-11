#pragma once

#include "CImage.h"
class LJH::CImage;
class CTextureKey {
	map<wstring, LJH::CImage*> m_mapImage;

public:
	CTextureKey();
	~CTextureKey();
	void Release();

	HRESULT InsertImage(const wstring& strImageKey, LJH::CImage* pImage);
	LJH::CImage* GetImage(const wstring& strImageKey);
};

class CImageManager
{
	DECLARE_SINGLETON(CImageManager)

	map<wstring, CTextureKey> m_mapTextureKey;

public:
	CImageManager();
	~CImageManager();

	void Release();
	
	HRESULT InsertImage(const wstring& strTextureKey, const wstring& strImageKey, const int& iStartIndexX, const int& iStartIndexY, const int& iMaxFrame);
	LJH::CImage* GetImage(const wstring& strTextureKey, const wstring& strImageKey);

};

