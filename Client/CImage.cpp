#include "stdafx.h"
#include "CImage.h"
#include "CTexture.h"
#include "GraphicDevice.h"
#include "CCameraManager.h"

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
	m_fTime += g_ETime;
	if (m_fTime >= m_fNexttime)
	{
		//시간을 프레임 경과 시간을 빼준다.
		m_fTime -= m_fNexttime;

		switch (m_ps)
		{
		case ANI_LOOP:
			if (m_iMaxFrame - 1 > m_iCurFrame)
				m_iCurFrame++;
			else {
				m_iCurFrame = 0;
			}
			break;
		case ANI_NOLOOP:
			if (m_uiRepeat == 1) break;
			if (m_iMaxFrame - 1 > m_iCurFrame)
				m_iCurFrame++;
			else {
				m_uiRepeat++;
			}
			break;
		}

	}
}


void LJH::CImage::Render(INFO* pInfo)
{
	CGraphicDevice* pGraphicDevice = CGraphicDevice::GetInstance();
	CCameraManager* pCameraManager = CCameraManager::GetInstance();

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
	_matrix matScale, matRotZ, matTrans, matWorld, matIdentity;
	// Parent
	D3DXMatrixIdentity(&matIdentity);
	bool m_bParent = false;
	if (matIdentity != pInfo->matParent) {
		m_bParent = true;
	}
	// Scale
	_vec3 vScale = pInfo->vScale;
	if (!m_bParent) {
		if (m_bFlip) vScale.x *= -1; // Flip
	}
	D3DXMatrixScaling(&matScale, vScale.x, vScale.y, 0.f);
	// Rot
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(pInfo->vRot.z));
	// Trans
	if (m_bParent) {
		D3DXMatrixTranslation(&matTrans,
			pInfo->vPos.x,
			pInfo->vPos.y,
			0.f);
	}
	else {
		POINT ptCam = pCameraManager->GetCam();
		D3DXMatrixTranslation(&matTrans,
			pInfo->vPos.x + (m_bCam ? -ptCam.x : 0),
			pInfo->vPos.y + (m_bCam ? -ptCam.y : 0),
			0.f);
	}
	// Set Transform
	matWorld = matScale * matRotZ * matTrans * pInfo->matParent;
	CGraphicDevice::GetInstance()->GetSprite()->SetTransform(&matWorld);

// Draw
	RECT rc = { iStartX, iStartY, iStartX + iImageCX, iStartY + iImageCY };
	CGraphicDevice::GetInstance()->GetSprite()->Draw(pTexInfo->pTexture, &rc, &vecCenter, nullptr, pInfo->color);
}
