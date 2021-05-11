#pragma once
#include "SingletonBase.h"

class CKeyManager : public SingletonBase<CKeyManager>
{
private:
	//Ű�ڵ� ���� ���ȴ��� Ȯ���� �Һ��� �迭
	bool bKeyDown[256];
	//���� ������ Ű��
	BYTE PrevKey;

public:
	CKeyManager();
	~CKeyManager();

	//�ѹ� ��������
	bool isKeyDown(int KeyCode);
	//��� ������ �ִ���
	bool StayKeyDown(int KeyCode);
	//������ ��������
	bool isKeyUp(int KeyCode);
};

