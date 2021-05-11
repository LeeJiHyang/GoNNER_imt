
// MFCToolView.h : CMFCToolView 클래스의 인터페이스
//

#pragma once

#include "CMap.h"

enum {
	TOOL_SEL_LINE,
	TOOL_SEL_COLL,
	TOOL_SEL_PORTALIN,
	TOOL_SEL_PORTALOUT,
	TOOL_SEL_MONSTER,
	TOOL_SEL_NUM
};

class CMFCToolDoc;
class CSingleTex;
class LJH::CMap;
class CMFCToolView : public CScrollView
{
protected: // serialization에서만 만들어집니다.
	CMFCToolView();
	DECLARE_DYNCREATE(CMFCToolView)
public:
// 특성입니다.
public:
	CMFCToolDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CMFCToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

	void LoadTexture();
	void ChangeNextLineDir();
	void RenderCursor();

	// Add Map Info
	void AddLineToMap(int iX, int iY);
	void AddCollToMap(RECT rc);
	void AddMobToMap(int iX, int iY, int iMobID);

	LJH::CMap* m_pMap = nullptr;
	LINE_DIR m_linedir = LINE_DIR_U;
	int m_iMonster = 0;

	int m_iSelX = 0;
	int m_iSelY = 0;
	void UpdateMouseSel();

	int m_iToolSel = TOOL_SEL_LINE;

	// Collision
	bool m_bStartColl = false;
	POINT m_ptStartColl;
	POINT m_ptEndColl;
};

#ifndef _DEBUG  // MFCToolView.cpp의 디버그 버전
inline CMFCToolDoc* CMFCToolView::GetDocument() const
   { return reinterpret_cast<CMFCToolDoc*>(m_pDocument); }
#endif

