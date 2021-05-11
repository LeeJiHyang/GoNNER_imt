#pragma once
class CObj
{
protected:
	INFO m_tInfo;
	OBJ::ID m_objID;
	float m_fSpeed = 0.f;
	float m_fAngle = 0.f;

	// Gravity
	bool m_bGravity = false;
	float m_fGravity = 0.f;
	float m_fForce = 13.f;

	// Dead
	bool m_bDeadStart = false;
	bool m_bDead = false;
	float m_fDeadStartTime = 0.f;

public:
	virtual void Ready() = 0;
	virtual void Update() = 0;
	virtual void LateUpdate() = 0;
	virtual void Render() = 0;
	virtual void Release() = 0;

	// Gravity
	void ActiveGravity();
	void DeactivateGravity();

	// Info
	INFO GetInfo() { return m_tInfo; }
	void SetInfo(INFO& info) { m_tInfo = info; }

	// Position
	float GetX();
	float GetY();
	void SetX(float fX);
	void SetY(float fY);
	void SetPos(float fX, float fY);

	// Size
	int GetCX();
	int GetCY();

	// Speed
	float GetSpeed();
	void SetSpeed(float fSpeed);

	// Collision
	RECT GetCol();

	// Angle
	float GetAngle() { return m_fAngle; }

	// Dead
	bool GetDeadStart() { return m_bDeadStart; }
	void SetDeadStart(bool bDeadStart);

	bool GetDead() { return m_bDead; }
	void SetDead(bool bDead);
		
};

