#pragma once
class CTexture
{
	TEXINFO* m_pTexInfo;

public:
	int m_iImageCX;
	int m_iImageCY;
	int m_iIndexMaxX;
	int m_iIndexMaxY;

	explicit CTexture(int iIndexMaxX, int iIndexMaxY);
	virtual ~CTexture();

	virtual void Release();

	HRESULT InsertTexture(const wstring& strFilePath);
	const TEXINFO * GetTexInfo();
};

