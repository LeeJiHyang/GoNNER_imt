// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>

// C 런타임 헤더 파일입니다.
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

// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
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
