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
	// ���������� ��ġ�� �ٷ� ��ü. 
	LPDIRECT3DDEVICE9 m_pDevice;
	// ��ġ�� ���� ���� ���� �� ��ġ�� ������ ��ü�� ������ com ��ü .
	LPDIRECT3D9 m_pSDK; 
	// com ��ü!- �׳� �ϳ� �ϳ� ��ǰó�� �����Ѵٶ� �����ϸ� �ǰڴ�. 

	LPD3DXSPRITE m_pSprite;
	LPD3DXFONT	m_pFont; 

};

