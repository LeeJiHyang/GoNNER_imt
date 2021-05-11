#pragma once
#include "SingletonBase.h"
class CCameraManager : public SingletonBase<CCameraManager>
{
	_vec3 vShowCam = { 0.f, 0.f, 0.f };
	_vec3 vGoalCam = { 0.f, 0.f, 0.f };

	POINT m_ptCam = { 0, 0 }; // 효과를 준 이후의 카메라

// Shake
	bool m_bShake = false;
	int m_iTimeStartShake = 0;
	int m_iTimeShake = 0; // ms

public:
	void Update();
	void LateUpdate();

	void Shake(int iTime);
	POINT GetCam() { return m_ptCam; }
};

