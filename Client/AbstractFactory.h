#pragma once

class CObj;

template<typename T>
class CAbstractFactory
{
public:
	static CObj* Create()
	{
		CObj* pObj = new T;
		pObj->Ready();
		return pObj;
	}
	static CObj* Create(float fX, float fY)
	{
		CObj* pObj = new T;
		pObj->Ready();
		pObj->SetPos(fX, fY);
		return pObj;
	}
};
