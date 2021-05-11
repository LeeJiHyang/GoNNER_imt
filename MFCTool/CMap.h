#pragma once
class CMFCToolView; 

namespace LJH {
	class CImage;

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
		vector<CMobData> m_vecMob;
		POINT m_ptPortalIn;
		POINT m_ptPortalOut;
	};

	class CMap final
	{
		int m_iMapSel = 0; // m_vecMapData 선택


	public:
		explicit CMap();
		virtual ~CMap();

		HRESULT Ready();
		void	Render();
		void	Release();
		
		void SetView(CMFCToolView* pView) { m_pView = pView; }
		void LoadMapData();
		void SaveMapData(HANDLE hHandle, int iMapSel);
		int GetSelectMap() { return m_iMapSel; }
		void SetSelectMap(int iSel) { m_iMapSel = iSel; } // m_vecMapData 선택

		LJH::CImage* m_ImageLine[1][8];
		LJH::CImage* m_ImagePortalIn;
		LJH::CImage* m_ImagePortalOut;
		LJH::CImage* m_ImageMonster[2];

		vector<CMapData> m_vecMapData;
		CMFCToolView* m_pView;
	};
}
