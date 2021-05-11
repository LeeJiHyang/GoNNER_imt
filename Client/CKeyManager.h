#pragma once
#include "SingletonBase.h"

class CKeyManager : public SingletonBase<CKeyManager>
{
private:
	//키코드 값들 눌렸는지 확인할 불변수 배열
	bool bKeyDown[256];
	//전에 눌렀던 키값
	BYTE PrevKey;

public:
	CKeyManager();
	~CKeyManager();

	//한번 눌렀는지
	bool isKeyDown(int KeyCode);
	//계속 누르고 있는지
	bool StayKeyDown(int KeyCode);
	//눌렀다 떼었는지
	bool isKeyUp(int KeyCode);
};

