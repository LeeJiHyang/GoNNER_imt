#pragma once
class CTexture;

namespace LJH {
	class CImage
	{
		CTexture* m_pTexture;
		wstring m_strImageKey;
		int m_iStartIndexX;
		int m_iStartIndexY;
		int m_iStartIndex;
		int m_iMaxFrame;
		int m_iCurFrame = 0;
		float m_fTime = 0.f;
		BOOL m_bFlip = false;

	public:
		CImage(CTexture* pTexture, const wstring& strImageKey, const int& iStartIndexX, const int& iStartIndexY, const int& iMaxFrame);

		const TEXINFO* GetTexInfo();
		void Render(INFO* pInfo);
		void Update();
		void InitFrame() { m_iCurFrame = 0; }
		void SetFlipY(bool b) { m_bFlip = b; }
	};
};