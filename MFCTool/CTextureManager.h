#pragma once
class CTexture;
class CTextureManager
{
	DECLARE_SINGLETON(CTextureManager)

	// ObjectKey
	map<wstring, CTexture*> m_mapTexture;

public:
	CTextureManager();
	~CTextureManager();

	void Release();

	const TEXINFO* GetTexInfo(const wstring& strObjectKey);
	HRESULT InsertTexture(const wstring& strFilePath, const wstring& strObjectKey, const int& iIndexMaxX, const int& iIndexMaxY);
	CTexture* GetTexture(const wstring& strObjectKey);

};

