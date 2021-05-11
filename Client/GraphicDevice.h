#pragma once
class CGraphicDevice
{
DECLARE_SINGLETON(CGraphicDevice)
private:
	CGraphicDevice();
	~CGraphicDevice();
public:
	LPDIRECT3DDEVICE9 GetDevice() { return m_pDevice;  }
	LPD3DXSPRITE GetSprite() { return m_pSprite; }
	LPD3DXFONT GetFont() { return m_pFont; }
public:
	HRESULT Ready();
public:
	void RenderBegin(); 
	void RenderEnd(HWND hWnd = nullptr); 

public:
	void Release(); 
private:
	// 실질적으로 장치를 다룰 객체. 
	LPDIRECT3DDEVICE9 m_pDevice;
	// 장치의 지원 수준 조사 및 장치를 제어할 객체를 생성할 com 객체 .
	LPDIRECT3D9 m_pSDK; 
	// com 객체!- 그냥 하나 하나 부품처럼 생각한다라 생각하면 되겠다. 

	LPD3DXSPRITE m_pSprite;
	LPD3DXFONT	m_pFont; 

};

