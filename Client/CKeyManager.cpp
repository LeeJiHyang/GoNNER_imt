#include "stdafx.h"
#include "CKeyManager.h"


CKeyManager::CKeyManager()
	:PrevKey(NULL)
{
	//0 으로 멤버 초기화
	ZeroMemory(bKeyDown, sizeof(bKeyDown));

}


CKeyManager::~CKeyManager()
{
}

bool CKeyManager::isKeyDown(int KeyCode)
{
	//이전에 누른적이 있던지 없던지 현재 눌렀는가
	if (GetAsyncKeyState(KeyCode) & 0x8000)
	{
		//이전에 눌린적이 없다면
		if (!bKeyDown[KeyCode])
		{
			//해당 키코드 배열값을 true로 초기화후 true값 반환
			bKeyDown[KeyCode] = true;
			return true;
		}
	}
	//이전에 누른적이 있던지 없던지 현재 눌려 있지 않다면
	else
	{
		bKeyDown[KeyCode] = false;
	}
	return false;



	return false;
}

bool CKeyManager::StayKeyDown(int KeyCode)
{
	//이전에 누른적이 있던지 없던지 현재 눌렀는가
	if (GetAsyncKeyState(KeyCode) & 0x8000)
	{
		PrevKey = KeyCode;
		return true;
	}
	return false;
}

bool CKeyManager::isKeyUp(int KeyCode)
{
	//누르고 있지 않다면
	if (!StayKeyDown(KeyCode))
	{
		//이전에 눌린 키값과 같을때
		if (PrevKey == KeyCode)
		{
			//이전키값 초기화
			PrevKey = NULL;
			return true;
		}
	}
	return false;
}
