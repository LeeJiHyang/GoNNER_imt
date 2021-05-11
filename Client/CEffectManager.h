#pragma once
#include "SingletonBase.h"
#include "CImage.h"
#include "CObj.h"

class CObj;
class CEffect;

class CEffectManager : public SingletonBase<CEffectManager>
{
	list<CEffect*> m_listEffect;

public:
	void Init();
	void Release();
	int Update();
	void LateUpdate();
	void Render();

	void AddEffect(const wstring& strTextureKey, const wstring& strImageKey, INFO info);
	vector<RECT> GetVecRectCol();
	list<RECT*> GetListRectCol();
};

