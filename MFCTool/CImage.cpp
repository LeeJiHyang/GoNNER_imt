#include "stdafx.h"
#include "CImage.h"
#include "CTexture.h"
#include "GraphicDevice.h"

LJH::CImage::CImage(CTexture* pTexture, const wstring & strImageKey, const int & iStartIndexX, const int & iStartIndexY, const int & iMaxFrame)
{
	m_pTexture = pTexture;
	m_strImageKey = strImageKey;
	m_iStartIndexX = iStartIndexX;
	m_iStartIndexY = iStartIndexY;
	m_iStartIndex = (iStartIndexY * m_pTexture->m_iIndexMaxX) + m_iStartIndexX;
	m_iMaxFrame = iMaxFrame;
}

const TEXINFO * LJH::CImage::GetTexInfo()
{
	return m_pTexture->GetTexInfo();
}

void LJH::CImage::Update()
{
	float fNexttime = 0.05f;

	m_fTime += g_ETime;
	if (m_fTime >= fNexttime)
	{
		//시간을 프레임 경과 시간을 빼준다.
		m_fTime -= fNexttime;

		if (m_iMaxFrame - 1 > m_iCurFrame)
			m_iCurFrame++;
		else {
			m_iCurFrame = 0;
		}
	}
}


void LJH::CImage::Render(INFO* pInfo)
{
	CGraphicDevice* pGraphicDevice = CGraphicDevice::GetInstance();

	const TEXINFO* pTexInfo = m_pTexture->GetTexInfo();

	// Size
	int iImageCX = m_pTexture->m_iImageCX;
	int iImageCY = m_pTexture->m_iImageCY;
	int iTextureCX = m_pTexture->m_iIndexMaxX * iImageCX;
	int iTextureCY = m_pTexture->m_iIndexMaxY * iImageCY;

	int iStartX = ((m_iStartIndex + m_iCurFrame) % m_pTexture->m_iIndexMaxX) * iImageCX;
	int iStartY = ((m_iStartIndex + m_iCurFrame) / m_pTexture->m_iIndexMaxX) * iImageCY;

	_vec3 vecCenter = { (FLOAT)(iImageCX >> 1), (FLOAT)(iImageCY >> 1), 0.f };

// Set Matrix
	_matrix matScale, matRotZ, matTrans, matWorld;
	// Scale
	_vec3 vSize = pInfo->vSize;
	if (m_bFlip) vSize.x *= -1; // Flip
	D3DXMatrixScaling(&matScale, vSize.x, vSize.y, 0.f);
	// Rot
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(pInfo->vRot.z));
	// Trans
	D3DXMatrixTranslation(&matTrans, pInfo->vPos.x, pInfo->vPos.y, 0.f);
	// Set Transform
	matWorld = matScale * matRotZ * matTrans * pInfo->matParent;
	CGraphicDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

// Draw
	RECT rc = { iStartX, iStartY, iStartX + iImageCX, iStartY + iImageCY };
	CGraphicDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, &rc, &vecCenter, nullptr, pInfo->color);
}
