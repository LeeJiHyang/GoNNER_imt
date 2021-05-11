#include "stdafx.h"
#include "CSceneManager.h"
#include "CScene.h"

CSceneManager::CSceneManager()
{
}

CSceneManager::~CSceneManager()
{
	DeleteAll();
}

CScene * CSceneManager::AddScene(string name, CScene * scene)
{
	if (!scene) return nullptr;
	m_mapScene.insert(make_pair(name, scene));

	return scene;
}

CScene * CSceneManager::ChangeScene(string name)
{
	// Ű Ž��
	SceneIter iter = m_mapScene.find(name);

	// Ű���� ������
	if (iter == m_mapScene.end()) return nullptr;
	// �ٲٷ��� ���� ���� ���� �����ϴٸ�
	if (iter->second == m_SceneCurrent) return m_SceneCurrent;

	if (iter->second->Ready())
	{
		if (m_SceneCurrent) m_SceneCurrent->Release();
		m_SceneCurrent = iter->second;

		return m_SceneCurrent;
	}
	return nullptr;
}

void CSceneManager::DeleteAll()
{
	//Ű Ž��
	SceneIter iter = m_mapScene.begin();

	for (iter; iter != m_mapScene.end();)
	{
		if (iter->second != nullptr)
		{
			//������� �����Ǿ�� �Ѵٸ�
			if (iter->second == m_SceneCurrent)
			{
				//release�� ȣ���Ѵ�.
				iter->second->Release();
			}
			SafeDelete(iter->second);
			iter = m_mapScene.erase(iter);
		}
		else
		{
			iter++;
		}
	}
	m_mapScene.clear();
}

void CSceneManager::Update()
{
	if (m_SceneCurrent) m_SceneCurrent->Update();
}

void CSceneManager::LateUpdate()
{
	if (m_SceneCurrent) m_SceneCurrent->LateUpdate();
}

void CSceneManager::Render()
{
	if (m_SceneCurrent) m_SceneCurrent->Render();
}
