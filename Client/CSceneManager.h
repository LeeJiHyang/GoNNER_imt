#pragma once
#include "SingletonBase.h"

class CScene;
class CSceneManager : public SingletonBase<CSceneManager>
{
	typedef map<string, CScene*>			SceneMap;
	typedef map<string, CScene*>::iterator	SceneIter;
	
	CScene*		m_SceneCurrent = nullptr;
	SceneMap	m_mapScene;

public:
	CSceneManager();
	virtual ~CSceneManager();

// Scene 조작
	CScene*	AddScene(string name, CScene* scene);
	CScene* ChangeScene(string name);
	void DeleteAll();

	CScene* GetCurrentScene() { return m_SceneCurrent; }

// Scene 호출
	void Update();
	void LateUpdate();
	void Render();
};

