#pragma once
#include "CScene.h"
class CLogo :
	public CScene
{
	// CScene을(를) 통해 상속됨
	virtual bool Ready() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
};

