#pragma once
#include "CObj.h"
#include "CImage.h"

class CBubble : public CObj
{
	INFO m_infoDraw;
	INFO m_infoGoal;
	float m_fStartTime;
	float m_fEndTime;

public:
	CBubble() {};
	CBubble(INFO& infoStart, INFO& infoGoal);
	virtual ~CBubble() {};

	// CObj을(를) 통해 상속됨
	virtual void Ready() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void Release() override;

	LJH::CImage* m_Image;
};

