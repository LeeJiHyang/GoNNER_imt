#pragma once
#include "CObj.h"
#include "CImage.h"

extern const int iPortalBodyNum;

enum PORTAL_TYPE {
	PORTAL_IN,
	PORTAL_OUT,
	PORTAL_NUM
};

class CPortalBody : public  CObj {
	LJH::CImage* m_Image;
public:
	CPortalBody();
	virtual ~CPortalBody();

	virtual void Ready();
	virtual void Update();
	virtual void LateUpdate();
	virtual void Render();
	virtual void Release();
};

class CPortal : public CObj {
	PORTAL_TYPE m_Type;
	INFO m_infoDraw;
	INFO m_infoGoal;
	float m_fChangeAngle = 10.f; // 원래 각도 대비 변경되는 각도
	list<float> m_listAngleBuff;

public:
	CPortal();
	virtual ~CPortal();

	// CObj을(를) 통해 상속됨
	virtual void Ready() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
	virtual void Release() override;

	LJH::CImage* m_ImageHead;
	LJH::CImage* m_ImageBody;
	list<CPortalBody> m_listBody;
	float m_fRatio = 1.f;

	void SetType(PORTAL_TYPE type);
	void SetRatio(float fRatio);

	// time
	float m_fTimeAngleChange = 3.f;
	float m_fLastAngleChange = 0.f;

	void SetPos(float fX, float fY);
};

class CMonster;
namespace LJH {
	class LJH::CImage;

	class CMobData {
	public:
		CMobData() {
			m_iMobID = 0;
			m_vPos = { 0.f, 0.f, 0.f };
		}
		int m_iMobID;
		_vec3 m_vPos;
	};
	class CMapData {
	public:
		int m_iType = 0; // 라인 모양
		vector<LINE> m_vecLine;
		vector<RECT> m_vecColl;
		vector<CMobData> m_vecMob; // ChangeMapData에서 CMonster 리스트에 넣어준 이후에는 사용하지 않음
		POINT m_ptPortalIn; // ChangeMapData에서 CPortal에 넘겨준 이후에는 사용하지 않음
		POINT m_ptPortalOut; // ChangeMapData에서 CPortal에 넘겨준 이후에는 사용하지 않음
	};

	class CMap : public CObj
	{
		int m_iMapSel = 0; // m_vecMapData 선택
		float m_fRatio = 0.7f;
		CPortal* m_pPortalIn;
		CPortal* m_pPortalOut;


	public:
		explicit CMap();
		virtual ~CMap();

		virtual void Ready();
		virtual void Render();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Release();

		void SetMonsterList(list<CObj*>* pMonsterList);
		list<CObj*>* m_pMonsterList;

		void LoadMapData();
		void SaveMapData(HANDLE hHandle, int iMapSel);
		int GetSelectMap() { return m_iMapSel; }
		void SetSelectMap(int iSel) { m_iMapSel = iSel; } // m_vecMapData 선택
		void ChangeMapData(int iSel);
		
		LJH::CImage* m_ImageLine[1][8];

		vector<CMapData> m_vecMapData;
		CMapData* m_pCurrMapData = nullptr;
		vector<RECT> GetVecCol() { return m_pCurrMapData->m_vecColl; }

		_vec3 GetStartPos() { return _vec3((float)m_pCurrMapData->m_ptPortalIn.x, (float)m_pCurrMapData->m_ptPortalIn.y, 0.f); }
	};
}
