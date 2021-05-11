// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �Ǵ� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <windows.h>

// C ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <time.h>

#include <d3d9.h>
//#pragma comment(lib, "d3d9.lib")
#include <d3dx9.h>
//#pragma comment(lib, "d3dx9.lib")

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <map>
using namespace std;

// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
#include "Total.h"
#include "enum.h"
#include "typedef.h"
#include "function.h"
#include "struct.h"
#include "define.h"

#include "GraphicDevice.h"

#include "AbstractFactory.h"

// Manager
// Time
#include "CTimeManager.h"
#define TIMEMANAGER CTimeManager::GetInstance()
//#define g_ETime CTimeManager::GetInstance()->GetElapsedTime()
#define g_ETime (1.0f / 60.f)
#include "CBubbleManager.h"
#define BUBBLEMANAGER CBubbleManager::GetInstance()
#include "CSoundManager.h"
#define SOUNDMANAGER SoundManager::GetInstance()


// Global Variable
extern POINT g_ptCam;
