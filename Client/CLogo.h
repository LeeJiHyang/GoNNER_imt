#pragma once
#include "CScene.h"
class CLogo :
	public CScene
{
	// CScene��(��) ���� ��ӵ�
	virtual bool Ready() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
};

