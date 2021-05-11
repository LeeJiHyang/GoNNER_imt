#pragma once

namespace LJH {
	class CImage;
	class CTextureKey {
		map<wstring, CImage*> m_mapImage;

	public:
		CTextureKey();
		~CTextureKey();
		void Release();

		HRESULT InsertImage(const wstring& strImageKey, CImage* pImage);
		CImage* GetImage(const wstring& strImageKey);
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
		CImage* GetImage(const wstring& strTextureKey, const wstring& strImageKey);
	};
}