#pragma once
#include "CMap.h"
class CObj;
class CPlayer;
class LJH::CMap;
class CMonster;
class CUI;
class CMainApp
{
public:
	explicit CMainApp();
	~CMainApp();

	void Ready(); 
	void Update(); 
	void LateUpdate(); 
	void Render(); 
	void Release(); 

private:
	void CreateGraphicDevice();
	void LoadTexture();
	void LoadSound();
	void InitScene();

	HDC m_hDC; 
	list<CObj*> m_listObject[OBJ::OBJ_END];

	CPlayer* m_pPlayer = nullptr;
	LJH::CMap* m_pMap = nullptr;
	CMonster* m_pMonster = nullptr;
	CUI* m_pUI = nullptr;
};

