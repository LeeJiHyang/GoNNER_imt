#include "stdafx.h"
#include "CBubbleManager.h"
#include "CBubble.h"
#include "CImage.h"
#include "CImageManager.h"

void CBubbleManager::AddBubble(int iNum, INFO info, float fAngle, _color color)
{
	float fScale;
	float fAngleEdit;

	for (int i = 0; i < iNum; ++i)
	{
		fScale = ((rand() % 30) + 70) * 0.01f;
		fAngleEdit = fAngle + ((rand() % 30) - 50);

		info.vDir.x = cosf(D3DXToRadian(fAngleEdit)) * 500.f;
		info.vDir.y = sinf(D3DXToRadian(fAngleEdit)) * 500.f;

		INFO infoStart;
		infoStart.vPos = info.vPos;
		infoStart.vDir = info.vDir;
		infoStart.vScale = { fScale, fScale, 0.f };
		infoStart.color = color;

		INFO infoGoal;
		infoGoal.vDir = { 0.f, -80.f, 0.f };
		infoGoal.vScale = { fScale * 0.3f, fScale * 0.3f, 0.f };
		infoGoal.color = color;

		// Effect
		CBubble* pBubble = new CBubble(infoStart, infoGoal);

		m_pBubbleList->emplace_back(pBubble);
	}
}
