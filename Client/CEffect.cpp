#include "stdafx.h"
#include "CEffect.h"

CEffect::CEffect()
{
	m_objID = OBJ::OBJ_EFFECT;
}

void CEffect::Ready()
{
}

void CEffect::Update()
{
	m_tInfo.vPos += m_tInfo.vDir * g_ETime;
	m_Image->Update();
}

void CEffect::LateUpdate()
{
}

void CEffect::Render()
{
	m_Image->Render(&m_tInfo);
}

void CEffect::Release()
{
	SafeDelete(m_Image);
}
