#pragma once
#include "SingletonBase.h"

class CBubbleManager : public SingletonBase<CBubbleManager>
{
	list<CObj*>* m_pBubbleList;
public:
	void SetBubbleList(list<CObj*>* pBubbleList) { m_pBubbleList = pBubbleList; }
	void AddBubble(int iNum, INFO info, float fAngle, _color color);
};

