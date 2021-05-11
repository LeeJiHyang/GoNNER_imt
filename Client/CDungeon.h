#pragma once
#include "CScene.h"
#include "CMap.h"
class CPlayer;
class CDungeon :
	public CScene
{
	CPlayer* m_pPlayer = nullptr;
	LJH::CMap* m_pMap = nullptr;

public:
	// CScene을(를) 통해 상속됨
	virtual bool Ready() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

	void ChangeMap(int i);

	void AddEffect_BulletHitLine(RECT& rc, CObj* pBullet);
	void AddEffect_BulletHitMonster(CObj* pMonster);
};
