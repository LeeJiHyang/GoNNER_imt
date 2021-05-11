#pragma once
#include "CObj.h"
#include "CImage.h"

enum MONSTER_STATE {
	MS_IDLE,
	MS_MOVE,
	MS_HIT,
	MS_NUM
};

enum MONSTER_DIRECTION {
	MD_LEFT,
	MD_RIGHT,
	MD_NUM
};

enum MONSTER_PATTERN {
	MP_NONE,
	MP_CHASE,
	MP_END
};

extern wstring strMonsterImageKey[2][MS_NUM];

class CMonster : public CObj
{
	list<CObj*>* m_pEffectList;
	LJH::CImage* m_Image[MS_NUM];
	int m_iMobID;
	MONSTER_STATE m_ms = MS_IDLE;
	MONSTER_DIRECTION m_md = MD_LEFT;
	MONSTER_PATTERN m_mp = MP_NONE;

	bool m_bDrawDeadEffect = false;

public:
	CMonster() {}
	CMonster(_vec3 vPos, int iMobID);
	virtual ~CMonster();

	// CObj을(를) 통해 상속됨
	virtual void Ready() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void Release() override;

	void SetEffectList(list<CObj*>* pEffectList);

	void DrawDeadEffect();

	// 
	bool m_bPatternOn = false;
	float m_fLastMove = 0.f;
	float m_fNextMove = 1.5f;
	float m_fActionTime = 1.f;
};

