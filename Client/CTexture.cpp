#include "stdafx.h"
#include "CTexture.h"
#include "CImage.h"
#include "GraphicDevice.h"

CTexture::CTexture(int iIndexMaxX, int iIndexMaxY)
{
	m_iIndexMaxX = iIndexMaxX;
	m_iIndexMaxY = iIndexMaxY;
}


CTexture::~CTexture()
{
	Release();
}

HRESULT CTexture::InsertTexture(const wstring & strFilePath)
{
	m_pTexInfo = new TEXINFO;
	ZeroMemory(m_pTexInfo, sizeof(TEXINFO));

	if (FAILED(D3DXGetImageInfoFromFile(strFilePath.c_str(), &m_pTexInfo->tImageInfo)))
	{
		ERR_MSG(L"이미지 정보 불러오기 실패");
		return E_FAIL;
	}

	if (FAILED(D3DXCreateTextureFromFileEx(
		CGraphicDevice::GetInstance()->GetDevice(),
		strFilePath.c_str(),
		m_pTexInfo->tImageInfo.Width,
		m_pTexInfo->tImageInfo.Height,
		m_pTexInfo->tImageInfo.MipLevels,
		0,
		m_pTexInfo->tImageInfo.Format,
		D3DPOOL_MANAGED,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		0,
		nullptr,
		nullptr,
		&m_pTexInfo->pTexture)))
	{
		wstring strErrMessage = strFilePath + L" Create Texture Failed";
		ERR_MSG(strErrMessage.c_str());
		return E_FAIL;
	}

	m_iImageCX = m_pTexInfo->tImageInfo.Width / m_iIndexMaxX;
	m_iImageCY = m_pTexInfo->tImageInfo.Height / m_iIndexMaxY;

	return S_OK;
}

void CTexture::Release()
{
	SafeDelete(m_pTexInfo);
}

const TEXINFO * CTexture::GetTexInfo()
{
	return m_pTexInfo;
}
