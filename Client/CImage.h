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
		bool m_bCam = true;
		float m_fNexttime = 0.05f;

		UINT m_uiRepeat = 0;
		PlayState m_ps = ANI_LOOP;

	public:
		CImage(CTexture* pTexture, const wstring& strImageKey, const int& iStartIndexX, const int& iStartIndexY, const int& iMaxFrame);

		const TEXINFO* GetTexInfo();
		void Render(INFO* pInfo);
		void Update();
		void InitFrame() { m_iCurFrame = 0; }
		void SetFlipY(bool b) { m_bFlip = b; }
		// Cam
		// true - 캠 적용, 캠에 따라 위치 변경됨 / false - 캠 적용 X, 처음 위치로 고정
		void SetCam(bool bOn) { m_bCam = bOn; }
		void SetTime(float fNexttime) { m_fNexttime = fNexttime; }
		// PS
		PlayState GetPS() { return m_ps; }
		void SetPS(PlayState ps) { m_ps = ps; }
		// Repeat
		UINT GetRepeatNum() { return m_uiRepeat; }
		void SetRepeatNum(UINT uiRepeat) { m_uiRepeat = uiRepeat; }
		// Frame
		int GetCurrentFrame() { return m_iCurFrame; }
		void SetCurrentFrame(int iCurFrame) {
			m_iCurFrame = iCurFrame;
		}
		int GetMaxFrame() { return m_iMaxFrame; }

	};
};
