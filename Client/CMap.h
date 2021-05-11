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
	float m_fChangeAngle = 10.f; // ���� ���� ��� ����Ǵ� ����
	list<float> m_listAngleBuff;

public:
	CPortal();
	virtual ~CPortal();

	// CObj��(��) ���� ��ӵ�
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
		int m_iType = 0; // ���� ���
		vector<LINE> m_vecLine;
		vector<RECT> m_vecColl;
		vector<CMobData> m_vecMob; // ChangeMapData���� CMonster ����Ʈ�� �־��� ���Ŀ��� ������� ����
		POINT m_ptPortalIn; // ChangeMapData���� CPortal�� �Ѱ��� ���Ŀ��� ������� ����
		POINT m_ptPortalOut; // ChangeMapData���� CPortal�� �Ѱ��� ���Ŀ��� ������� ����
	};

	class CMap : public CObj
	{
		int m_iMapSel = 0; // m_vecMapData ����
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
		void SetSelectMap(int iSel) { m_iMapSel = iSel; } // m_vecMapData ����
		void ChangeMapData(int iSel);
		
		LJH::CImage* m_ImageLine[1][8];

		vector<CMapData> m_vecMapData;
		CMapData* m_pCurrMapData = nullptr;
		vector<RECT> GetVecCol() { return m_pCurrMapData->m_vecColl; }

		_vec3 GetStartPos() { return _vec3((float)m_pCurrMapData->m_ptPortalIn.x, (float)m_pCurrMapData->m_ptPortalIn.y, 0.f); }
	};
}
