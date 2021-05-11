#include "stdafx.h"
#include "CScene.h"
#include "CPlayer.h"
#include "CMap.h"

void CScene::StartFadein()
{
	m_iFadeout = 255;
}

void CScene::Fadein(HDC hDC)
{
	//Graphics g(hDC);
	//SolidBrush brush(Color(m_iFadeout, 0, 0, 0));
	//g.FillRectangle(&brush, 0, 0, WINCX, WINCY);
	m_iFadeout -= 10;
	if (m_iFadeout < 0) m_iFadeout = 0;
}

bool CScene::IsEndFadein()
{
	if (m_iFadeout <= 0) return true;
	
	return false;
}

void CScene::StartFadeout()
{
	m_iFadeout = 0;
}

void CScene::Fadeout(HDC hDC)
{
	//Graphics g(hDC);
	//SolidBrush brush(Color(m_iFadeout, 0, 0, 0));
	//g.FillRectangle(&brush, 0, 0, WINCX, WINCY);
	m_iFadeout += 10;
	if (m_iFadeout > 255) m_iFadeout = 255;
}

bool CScene::IsEndFadeout()
{
	if (m_iFadeout >= 255) return true;
	
	return false;
}
