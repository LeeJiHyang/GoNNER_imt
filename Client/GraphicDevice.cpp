#include "stdafx.h"
#include "GraphicDevice.h"

IMPLEMENT_SINGLETON(CGraphicDevice)

CGraphicDevice::CGraphicDevice()
{
}


CGraphicDevice::~CGraphicDevice()
{
	Release(); 
}

HRESULT CGraphicDevice::Ready()
{
	// ��ġ �ʱ�ȭ ����. 
	//1.SDk �� ���� ����� �ְ� 
	//2.��ġ�� ���� ������ �����Ѵ�. SDk
	//3. ���� ���ؿ� �´� �İ�ü(���������� ��ġ�� ������ ��ü)�� ����. 
	D3DCAPS9 DeviceCaps; 
	ZeroMemory(&DeviceCaps, sizeof(D3DCAPS9));

	//1.��ġ�� ������ ������ �İ�ü�� ����. 
	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION); 

	if (FAILED(m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &DeviceCaps)))
	{
		// ���� �������� ���� �޽����� �־��� ��. 
		ERR_MSG(L"Get DeviceCaps Failed"); 

		return E_FAIL; 
	}
	DWORD vp = 0; 
	if (DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	D3DPRESENT_PARAMETERS d3dpp; 
	ZeroMemory(&d3dpp, sizeof(d3dpp)); 

	//�� ������ ���� ���� ũ��. 
	d3dpp.BackBufferWidth = WINCX;
	d3dpp.BackBufferHeight = WINCY;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;

	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;

	
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = g_hWND;
	// �����̸� ��üȭ��, ���̸�  â����� ����ϰڴ�. 
	d3dpp.Windowed = TRUE;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	//3. �����ϰ� ������ �����͸� ������ ���� ��ġ�� ������ �İ�ü�� ��������!
	if (FAILED(m_pSDK->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hWND, vp, &d3dpp, &m_pDevice)))
	{
		ERR_MSG(L"Failed Creating Device");
		return E_FAIL; 
	}
	if (FAILED(D3DXCreateSprite(m_pDevice,&m_pSprite)))
	{
		ERR_MSG(L"Failed Creating Sprite");
		return E_FAIL;
	}
	D3DXFONT_DESCW tFontInfo; 
	ZeroMemory(&tFontInfo, sizeof(D3DXFONT_DESCW)); 
	tFontInfo.Height = 20;
	tFontInfo.Width = 20;
	tFontInfo.Weight = FW_HEAVY;
	tFontInfo.CharSet = HANGUL_CHARSET;
	lstrcpy(tFontInfo.FaceName,L"�����ý��丮");
	if (FAILED(D3DXCreateFontIndirect(m_pDevice, &tFontInfo, &m_pFont)))
	{
		ERR_MSG(L"��Ʈ ���� ����"); 
		return E_FAIL; 
	}
	return S_OK;
	//������ ���� - �ĸ���۸� ����. -> �׸��� -> �ĸ���ۿ� ���� ���۸� ��ü�Ѵ�. 

}

void CGraphicDevice::RenderBegin()
{
	m_pDevice->Clear(0,nullptr,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_ARGB(255, 33, 16, 28), 0.f, 0);
	m_pDevice->BeginScene();
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
}

void CGraphicDevice::RenderEnd(HWND hWnd /*= nullptr*/)
{
	m_pSprite->End();
	m_pDevice->EndScene();
	m_pDevice->Present(nullptr, nullptr, hWnd, nullptr);
}

void CGraphicDevice::Release()
{
	SafeRelease(m_pFont); 
	SafeRelease(m_pSprite); 
	SafeRelease(m_pDevice); 
	SafeRelease(m_pSDK); 
}
