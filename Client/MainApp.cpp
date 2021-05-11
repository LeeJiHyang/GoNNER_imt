#include "stdafx.h"
#include "MainApp.h"
#include "GraphicDevice.h"
#include "CTextureManager.h"
#include "CImageManager.h"
#include "CSceneManager.h"
#include "CCameraManager.h"
#include "CLogo.h"
#include "CDungeon.h"
#include "CPlayer.h"
#include "CMap.h"
#include "CMonster.h"
#include "CUI.h"


POINT g_ptCam = {};

CMainApp::CMainApp()
{
}


CMainApp::~CMainApp()
{
	Release(); 
}

void CMainApp::Ready()
{
// rand
	srand((unsigned int)(time(NULL)));

// hDC
	m_hDC = GetDC(g_hWND); 

// Graphic Device
	CreateGraphicDevice();

// Texture
	LoadTexture();

// Sound
	LoadSound();

// Set Cam
	g_ptCam.x = 0;
	g_ptCam.y = 0;

// Object
	// Create
	CObj* pObj = nullptr;

	// OBJ_PLAYER
	pObj = CAbstractFactory<CPlayer>::Create();
	m_pPlayer = dynamic_cast<CPlayer*>(pObj);
	m_listObject[OBJ::OBJ_PLAYER].emplace_back(pObj);

	// OBJ_MAP
	pObj = CAbstractFactory<LJH::CMap>::Create();
	m_pMap = dynamic_cast<LJH::CMap*>(pObj);
	m_listObject[OBJ::OBJ_MAP].emplace_back(pObj);

	// OBJ_MONSTER
	pObj = CAbstractFactory<CMonster>::Create();
	m_pMonster = dynamic_cast<CMonster*>(pObj);
	m_listObject[OBJ::OBJ_MONSTER].emplace_back(pObj);

	// OBJ_UI
	pObj = CAbstractFactory<CUI>::Create();
	m_pUI = dynamic_cast<CUI*>(pObj);
	m_listObject[OBJ::OBJ_UI].emplace_back(pObj);

	// Set Object
	m_pPlayer->SetMap(m_pMap);
	m_pUI->SetPlayer(m_pPlayer);

// Scene
	InitScene();
}

void CMainApp::Update()
{
	CSceneManager::GetInstance()->Update();
	CCameraManager::GetInstance()->Update();
}

void CMainApp::LateUpdate()
{
	CSceneManager::GetInstance()->LateUpdate();
	CCameraManager::GetInstance()->LateUpdate();
}

void CMainApp::Render()
{
	CGraphicDevice::GetInstance()->RenderBegin();

	CSceneManager::GetInstance()->Render();

	CGraphicDevice::GetInstance()->RenderEnd();
}

void CMainApp::Release()
{
	// Object
	for (int i = 0; i < OBJ::OBJ_END; i++) {
		for (auto pObj : m_listObject[i]) {
			SafeDelete(pObj);
		}
		m_listObject[i].clear();
	}
// Release Singleton
	LJH::CImageManager::DestroyInstance();
	CTextureManager::DestroyInstance();
	CGraphicDevice::DestroyInstance();
	TIMEMANAGER->DestroyInstance();
	BUBBLEMANAGER->DestroyInstance();

// DC
	ReleaseDC(g_hWND, m_hDC);
}

void CMainApp::CreateGraphicDevice()
{
	if (FAILED(CGraphicDevice::GetInstance()->Ready()))
	{
		ERR_MSG(L"그래픽 디바이스 생성 실패...");
		return;
	}
}

void CMainApp::LoadTexture()
{
	CTextureManager* pTextureManager = CTextureManager::GetInstance();
	LJH::CImageManager* pImageManager = LJH::CImageManager::GetInstance();

	// 텍스쳐 형식
	// 1. (복수) 규칙이 있는 것
	// 텍스쳐 추가 시 IndexMaxX, IndexMaxY 필요
	// 이미지 추가 시 Texture Key, Image Key, 시작인덱스(X, Y), MaxFrame 필요
	// 2. (복수) 규칙이 없는 것
	// 3. (단수) 한 장인 것 - 2번이랑 같은식으로 해도 될... 듯...

	// C

// Player
	if (FAILED(pTextureManager->InsertTexture(L"../resource/image/player.png", L"player", 16, 16))) return;
	if (FAILED(pImageManager->InsertImage(L"player", L"idle", 0, 0, 6))) return;
	if (FAILED(pImageManager->InsertImage(L"player", L"walk", 6, 0, 7))) return;
	if (FAILED(pImageManager->InsertImage(L"player", L"head_skull", 0, 5, 6))) return;
	if (FAILED(pImageManager->InsertImage(L"player", L"back_ammo", 10, 9, 6))) return;

	if (FAILED(pTextureManager->InsertTexture(L"../resource/image/weapons.png", L"weapons", 16, 16))) return;
	if (FAILED(pImageManager->InsertImage(L"weapons", L"rifle", 0, 0, 6))) return;
	if (FAILED(pImageManager->InsertImage(L"weapons", L"effect1", 6, 5, 6))) return;
	if (FAILED(pImageManager->InsertImage(L"weapons", L"bullet1", 0, 10, 6))) return;

	if (FAILED(pTextureManager->InsertTexture(L"../resource/image/line.png", L"line", 16, 16))) return;
	if (FAILED(pImageManager->InsertImage(L"line", L"line00_0", 8, 0, 1))) return;
	if (FAILED(pImageManager->InsertImage(L"line", L"line00_1", 9, 0, 1))) return;
	if (FAILED(pImageManager->InsertImage(L"line", L"line00_2", 10, 0, 1))) return;
	if (FAILED(pImageManager->InsertImage(L"line", L"line00_3", 11, 0, 1))) return;
	if (FAILED(pImageManager->InsertImage(L"line", L"line00_4", 12, 0, 1))) return;
	if (FAILED(pImageManager->InsertImage(L"line", L"line00_5", 13, 0, 1))) return;
	if (FAILED(pImageManager->InsertImage(L"line", L"line00_6", 14, 0, 1))) return;
	if (FAILED(pImageManager->InsertImage(L"line", L"line00_7", 15, 0, 1))) return;

	if (FAILED(pTextureManager->InsertTexture(L"../resource/image/smallenemies.png", L"smallenemies", 16, 16))) return;
	if (FAILED(pImageManager->InsertImage(L"smallenemies", L"enemy00_idle", 0, 0, 6))) return;
	if (FAILED(pImageManager->InsertImage(L"smallenemies", L"enemy00_move", 6, 0, 6))) return;
	if (FAILED(pImageManager->InsertImage(L"smallenemies", L"enemy00_hit", 12, 0, 4))) return;

	if (FAILED(pTextureManager->InsertTexture(L"../resource/image/bigsprites.png", L"bigsprites", 8, 8))) return;
	if (FAILED(pImageManager->InsertImage(L"bigsprites", L"circleparticle", 2, 6, 6))) return;
	if (FAILED(pImageManager->InsertImage(L"bigsprites", L"circleparticleend", 0, 7, 6))) return;
	
	if (FAILED(pTextureManager->InsertTexture(L"../resource/image/bigsprites2.png", L"bigsprites2", 16, 16))) return;
	if (FAILED(pImageManager->InsertImage(L"bigsprites2", L"portalin_idle", 12, 2, 6))) return;
	if (FAILED(pImageManager->InsertImage(L"bigsprites2", L"portalout_idle", 0, 0, 6))) return;
	if (FAILED(pImageManager->InsertImage(L"bigsprites2", L"portalbody_1", 4, 1, 6))) return;
	if (FAILED(pImageManager->InsertImage(L"bigsprites2", L"portalbody_2", 10, 1, 6))) return;
	if (FAILED(pImageManager->InsertImage(L"bigsprites2", L"portalbody_3", 0, 2, 6))) return;
	if (FAILED(pImageManager->InsertImage(L"bigsprites2", L"portalbody_4", 6, 2, 6))) return;
	if (FAILED(pImageManager->InsertImage(L"bigsprites2", L"textballoon", 8, 3, 6))) return;

	if (FAILED(pTextureManager->InsertTexture(L"../resource/image/bloodParticles.png", L"bloodParticles", 16, 16))) return;
	if (FAILED(pImageManager->InsertImage(L"bloodParticles", L"bullethitline1", 10, 1, 8))) return;
	if (FAILED(pImageManager->InsertImage(L"bloodParticles", L"bullethitline2", 1, 3, 8))) return;
	if (FAILED(pImageManager->InsertImage(L"bloodParticles", L"monsterhitskulleff", 5, 5, 16))) return;
	if (FAILED(pImageManager->InsertImage(L"bloodParticles", L"monsterhitshoteff", 5, 8, 9))) return;
	if (FAILED(pImageManager->InsertImage(L"bloodParticles", L"monsterhitextend1eff", 0, 0, 15))) return;
	if (FAILED(pImageManager->InsertImage(L"bloodParticles", L"monsterhitextend2eff", 16, 0, 11))) return;

	if (FAILED(pTextureManager->InsertTexture(L"../resource/image/sprites.png", L"sprites", 16, 16))) return;
	if (FAILED(pImageManager->InsertImage(L"sprites", L"hp", 6, 9, 6))) return;
	if (FAILED(pImageManager->InsertImage(L"sprites", L"questionmark", 2, 6, 6))) return;
}

void CMainApp::LoadSound()
{
	SOUNDMANAGER->addSound("jump", "../resource/sound/sfx_jump.wav", false);
	SOUNDMANAGER->addSound("bgm", "../resource/sound/Caves Bass+Keys 3.1.wav", true);
	SOUNDMANAGER->addSound("bullet", "../resource/sound/sfx_bullet_01.wav", false);
	SOUNDMANAGER->addSound("wallhit", "../resource/sound/bounce plosion.wav", false);
	SOUNDMANAGER->addSound("die", "../resource/sound/sfx_die.wav", false);
	
}

void CMainApp::InitScene()
{
	CSceneManager* pSceneManager = CSceneManager::GetInstance();
	CScene* scene;

	scene = new CLogo();
	dynamic_cast<CLogo*>(scene)->SetObjectList(m_listObject);
	pSceneManager->AddScene("Logo", scene);

	scene = new CDungeon();
	dynamic_cast<CDungeon*>(scene)->SetObjectList(m_listObject);
	pSceneManager->AddScene("Dungeon", scene);

	pSceneManager->ChangeScene("Dungeon");
}
