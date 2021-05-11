#pragma once
#include "CObj.h"
#include "CImage.h"

class CPlayer;
class CUI :
	public CObj
{
	CPlayer* m_pPlayer;
	vector<LJH::CImage*> m_vecImageHP;
	vector<LJH::CImage*> m_vecImageBack;
	vector<LJH::CImage*> m_vecImageBullet;


public:
	CUI();

	// CObj을(를) 통해 상속됨
	virtual void Ready() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void Release() override;

	void SetPlayer(CPlayer* pPlayer) { m_pPlayer = pPlayer; }
};

