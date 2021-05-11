#pragma once
#include "CObj.h"
#include "CImage.h"
#include "CMap.h"

class CPlayer;
class LJH::CImage;
class LJH::CMap;
class CBullet;
class CBalloon : public CObj {
	LJH::CImage* m_ImageBalloon;
	LJH::CImage* m_ImageBullet;
	LJH::CImage* m_ImageMark;
	float m_fStartTime = 0.f;
	float m_fShowTime = 0.5f;
	bool m_bFlip = false;
	CPlayer* m_pPlayer = nullptr;

public:
	CBalloon();
	virtual ~CBalloon();

	// CObj을(를) 통해 상속됨
	virtual void Ready() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void Release() override;
	
	void SetFlip(bool bFlip) { m_bFlip = bFlip; }
	void SetPlayer(CPlayer* pPlayer) { m_pPlayer = pPlayer; }
};

class CPlayer : public CObj
{
	LJH::CMap* m_pMap = nullptr;
	list<CObj*>* m_pBulletList = nullptr;
	list<CObj*>* m_pEffectList = nullptr;

	// Image
	LJH::CImage* m_pImageIdle = nullptr;
	LJH::CImage* m_pImageWalk = nullptr;
	LJH::CImage* m_pImageHead = nullptr;
	LJH::CImage* m_pImageBack = nullptr;
	LJH::CImage* m_pImageWeapon = nullptr;

	LJH::CImage* m_pCurrentImage = nullptr;

	// Info
	INFO m_tInfoHead;
	INFO m_tInfoBack;
	INFO m_tInfoWeapon;

	// State
	enum PLAYER_STATE {
		PS_IDLE,
		PS_WALK,
		PS_JUMP,
		PS_NUM
	};
	PLAYER_STATE m_ps = PS_IDLE;

	enum PLAYER_DIRECTION {
		PD_LEFT,
		PD_RIGHT,
		PD_NUM
	};
	PLAYER_DIRECTION m_pd = PD_LEFT;

	// Jump
	bool m_bJump = false;
	int m_iJumpCount = 0;

	// Attack
	float m_fAttackLastTime = 0.f;
	float m_fAttackPerSecond = 0.2f;

	// HP
	int m_iHP = 5;
	int m_iHPMax = 5;

	// Bullet
	int m_iBulletMax = 20;
	int m_iBullet = 20;

	// Balloon List
	list<CBalloon*> m_listBalloon;
public:
	CPlayer();
	~CPlayer();

	virtual void Ready();
	virtual void Update();
	virtual void LateUpdate();
	virtual void Render();
	virtual void Release();

	// Set Object
	void SetMap(LJH::CMap* pMap) { m_pMap = pMap; }
	void SetBulletList(list<CObj*>* pBulletList) { m_pBulletList = pBulletList; }
	void SetEffectList(list<CObj*>* pEffectList) { m_pEffectList = pEffectList; }

	// Attack
	void Attack();

	// Charge Bullet
	void ChargeBullet();

	// Gravity (재정의)
	void ActiveGravity();
	void DeactivateGravity();

	// HP
	int GetHP() { return m_iHP; }
	void SetHp(int iHP) { m_iHP = iHP; }
	void AddHp(int iHP) { m_iHP += iHP; }
	int GetHPMax() { return m_iHPMax; }
	int SetHPMax(int iHPMax) { m_iHPMax = iHPMax; }

	// Bullet
	int GetBullet() { return m_iBullet; }
	void SetBullet(int iBullet) { m_iBullet = iBullet; }
	void AddBullet(int iBullet) { m_iBullet += iBullet; }
	int GetBulletMax() { return m_iBulletMax; }
	int SetBulletMax(int iBulletMax) { m_iBulletMax = iBulletMax; }

	// Text Balloon
	void TextBalloon_NoBullet();

};

