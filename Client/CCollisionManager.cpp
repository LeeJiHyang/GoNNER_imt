#include "stdafx.h"
#include "CCollisionManager.h"
#include "CObj.h"

CCollisionManager::CCollisionManager()
{
}


CCollisionManager::~CCollisionManager()
{
}

bool CCollisionManager::CollisionRect(list<RECT*>& rDstList, list<CObj*>& rSrcList, CObj** pObj/* = nullptr*/)
{
	RECT rc = {};
	for (auto& rDstObject : rDstList)
	{
		for (auto& rSrcObject : rSrcList)
		{
			if (IntersectRect(&rc, rDstObject, &rSrcObject->GetCol()))
			{
				if (pObj != nullptr) {
					*pObj = rSrcObject;
				}
				return true;
			}
		}
	}
	return false;
}

void CCollisionManager::CollisionRect(list<CObj*>& rDstList, list<CObj*>& rSrcList)
{
	RECT rc = {};
	for (auto& rDstObject : rDstList)
	{
		if (rDstObject->GetDead() ||
			rDstObject->GetDeadStart())
			continue;
		for (auto& rSrcObject : rSrcList)
		{
			if (rSrcObject->GetDead() ||
				rSrcObject->GetDeadStart())
				continue;
			if (IntersectRect(&rc, &rDstObject->GetCol(), &rSrcObject->GetCol()))
			{
				rDstObject->SetDeadStart(true);
				rSrcObject->SetDeadStart(true);
			}
		}
	}
}

void CCollisionManager::CollisionRect(vector<CObj*>& rDstList, list<CObj*>& rSrcList)
{
	RECT rc = {};
	for (auto& rDstObject : rDstList)
	{
		for (auto& rSrcObject : rSrcList)
		{
			if (IntersectRect(&rc, &rDstObject->GetCol(), &rSrcObject->GetCol()))
			{
				//rDstObject->Set_Dead();
				//rSrcObject->Set_Dead();
			}
		}
	}
}

void CCollisionManager::CollisionRect(vector<RECT*>& rDstList, list<CObj*>& rSrcList)
{
	RECT rc = {};
	for (auto& rDstObject : rDstList)
	{
		for (auto& rSrcObject : rSrcList)
		{
			if (IntersectRect(&rc, rDstObject, &rSrcObject->GetCol()))
			{
				//rDstObject->Set_Dead();
				//rSrcObject->Set_Dead();
			}
		}
	}
}

void CCollisionManager::CollisionSphere(list<CObj*>& rDstList, list<CObj*>& rSrcList)
{
	for (auto& rDstObject : rDstList)
	{
		for (auto& rSrcObject : rSrcList)
		{
			if (CheckSphere(rDstObject, *rSrcObject))
			{
				//rDstObject->Set_Dead();
				//rSrcObject->Set_Dead();
			}
		}
	}
}

void CCollisionManager::CollisionRectEX(list<CObj*>& rDstList, list<CObj*>& rSrcList)
{
	float fMoveX = 0.f, fMoveY = 0.f;

	for (auto& rDstObject : rDstList)
	{
		for (auto& rSrcObject : rSrcList)
		{
			if (CheckRect(rDstObject, *rSrcObject, &fMoveX, &fMoveY))
			{
				float fX = rSrcObject->GetX();
				float fY = rSrcObject->GetY();

				if (fMoveX > fMoveY)
				{
					if (fY < rDstObject->GetY())
						fMoveY *= -1.f;

					rSrcObject->SetPos(fX, fY + fMoveY);
				}
				else
				{
					if (fX < rDstObject->GetX())
						fMoveX *= -1.f;
					rSrcObject->SetPos(fX + fMoveX, fY);
				}
			}
		}
	}
}

bool CCollisionManager::CollisionRectEX(vector<RECT>& rDstList, list<CObj*>& rSrcList, CObj* pObj /*= nullptr*/)
{
	bool bReturn = false;
	float fMoveX = 0.f, fMoveY = 0.f;

	for (auto& rDstObject : rDstList)
	{
		for (auto& rSrcObject : rSrcList)
		{
			if (CheckRect(&rDstObject, *rSrcObject, &fMoveX, &fMoveY))
			{
				float fX = rSrcObject->GetX();
				float fY = rSrcObject->GetY();

				if (fMoveX > fMoveY)
				{
					if (fY < (rDstObject.top + (GetCY(rDstObject) >> 1)))
						fMoveY *= -1.f;

					rSrcObject->SetPos(fX, fY + fMoveY);
					rSrcObject->DeactivateGravity();
					bReturn = true;
				}
				else
				{
					if (fX < (rDstObject.left + (GetCX(rDstObject) >> 1)))
						fMoveX *= -1.f;
					rSrcObject->SetPos(fX + fMoveX, fY);
				}
			}
		}
	}

	return bReturn;
}

bool CCollisionManager::CheckRect(CObj * pDstObject, CObj & rSrcObject, float * pMoveX, float * pMoveY)
{
	// 1.�簢�� �ΰ��� x�������� �������� ���� ����. 
	float fRadiusSumX = static_cast<float>((pDstObject->GetCX() >> 1) + (rSrcObject.GetCX() >> 1));
	//2.�簢�� �ΰ��� y�������� �������� ���� ����. 
	float fRadiusSumY = static_cast<float>((pDstObject->GetCY() >> 1) + (rSrcObject.GetCY() >> 1));

	// x��� y���� �Ÿ� ����. 
	float fDistX = fabs(pDstObject->GetX() - rSrcObject.GetX());
	float fDistY = fabs(pDstObject->GetY() - rSrcObject.GetY());

	// ���� �ΰ��� �Ÿ��� �������� ���� ���� ��. 
	if (fDistX <= fRadiusSumX && fDistY <= fRadiusSumY)
	{
		*pMoveX = fRadiusSumX - fDistX;
		*pMoveY = fRadiusSumY - fDistY;
		return true;
	}
	// �ΰ��� �Ÿ��� �� ª�ٶ�� ? �浹. 
	return false;
}

bool CCollisionManager::CheckRect(RECT * pDstRECT, CObj & rSrcObject, float * pMoveX, float * pMoveY)
{
	// 1.�簢�� �ΰ��� x�������� �������� ���� ����. 
	float fRadiusSumX = static_cast<float>((GetCX(*pDstRECT) >> 1) + (rSrcObject.GetCX() >> 1));
	//2.�簢�� �ΰ��� y�������� �������� ���� ����. 
	float fRadiusSumY = static_cast<float>((GetCY(*pDstRECT) >> 1) + (rSrcObject.GetCY() >> 1));

	// x��� y���� �Ÿ� ����. 
	float fDistX = fabs((pDstRECT->right - (GetCX(*pDstRECT) >> 1)) - rSrcObject.GetX());
	float fDistY = fabs((pDstRECT->bottom - (GetCY(*pDstRECT) >> 1)) - rSrcObject.GetY());

	// ���� �ΰ��� �Ÿ��� �������� ���� ���� ��. 
	if (fDistX <= fRadiusSumX && fDistY <= fRadiusSumY)
	{
		*pMoveX = fRadiusSumX - fDistX;
		*pMoveY = fRadiusSumY - fDistY;
	// �ΰ��� �Ÿ��� �� ª�ٶ�� ? �浹. 
		return true;
	}
	return false;
}

bool CCollisionManager::CheckSphere(CObj * pDstObject, CObj & rSrcObject)
{
	//float fRadiusSum = static_cast<float>(pDstObject->GetCX() >> 1) + (rSrcObject->GetCY() >> 1));
	//float fX = pDstObject->Get_Info()->fX - rSrcObject.Get_Info()->fX;
	//float fY = pDstObject->Get_Info()->fY - rSrcObject.Get_Info()->fY;
	//float fDist = sqrtf(fX * fX + fY * fY);

	//return fDist < fRadiusSum;
	return false;
}
