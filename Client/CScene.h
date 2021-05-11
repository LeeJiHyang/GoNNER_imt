#pragma once

#include "CMap.h"
class CPlayer;
class CMap;
class CObj;
enum SceneState
{
	SS_START,
	SS_RUN,
	SS_END,
	SS_NUM
};

class CScene
{
	int m_iFadeout = 0;

protected:
	SceneState m_ss = SS_START;

	list<CObj*> m_listObject[OBJ::OBJ_END];

public:
	CScene() {};
	virtual ~CScene() {};

	virtual bool Ready() = 0;
	virtual void Release() = 0;
	virtual void Update() = 0;
	virtual void LateUpdate() = 0;
	virtual void Render() = 0;

	void SetObjectList(list<CObj*> listObject[OBJ::OBJ_END]) {
		for (int i = 0; i < OBJ::OBJ_END; i++) {
			m_listObject[i] = listObject[i];
		}
	}

	void StartFadein();
	void Fadein(HDC hDC);
	bool IsEndFadein();
	void StartFadeout();
	void Fadeout(HDC hDC);
	bool IsEndFadeout();
};

