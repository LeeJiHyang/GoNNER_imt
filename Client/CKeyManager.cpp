#include "stdafx.h"
#include "CKeyManager.h"


CKeyManager::CKeyManager()
	:PrevKey(NULL)
{
	//0 ���� ��� �ʱ�ȭ
	ZeroMemory(bKeyDown, sizeof(bKeyDown));

}


CKeyManager::~CKeyManager()
{
}

bool CKeyManager::isKeyDown(int KeyCode)
{
	//������ �������� �ִ��� ������ ���� �����°�
	if (GetAsyncKeyState(KeyCode) & 0x8000)
	{
		//������ �������� ���ٸ�
		if (!bKeyDown[KeyCode])
		{
			//�ش� Ű�ڵ� �迭���� true�� �ʱ�ȭ�� true�� ��ȯ
			bKeyDown[KeyCode] = true;
			return true;
		}
	}
	//������ �������� �ִ��� ������ ���� ���� ���� �ʴٸ�
	else
	{
		bKeyDown[KeyCode] = false;
	}
	return false;



	return false;
}

bool CKeyManager::StayKeyDown(int KeyCode)
{
	//������ �������� �ִ��� ������ ���� �����°�
	if (GetAsyncKeyState(KeyCode) & 0x8000)
	{
		PrevKey = KeyCode;
		return true;
	}
	return false;
}

bool CKeyManager::isKeyUp(int KeyCode)
{
	//������ ���� �ʴٸ�
	if (!StayKeyDown(KeyCode))
	{
		//������ ���� Ű���� ������
		if (PrevKey == KeyCode)
		{
			//����Ű�� �ʱ�ȭ
			PrevKey = NULL;
			return true;
		}
	}
	return false;
}
